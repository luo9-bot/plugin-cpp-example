#include "main.h"

using namespace luo9::command;
using namespace luo9::bus;

// Message handlers

void handle_private_msg(uint64_t user_id, const std::string& msg) {
    auto reply = [user_id](const std::string& text) {
        luo9::bot::send_private_msg(user_id, text);
    };

    // /echo <text>
    auto cmd = Command::parse(msg, "echo", PrefixMode::Required('/'));
    if (!cmd.empty()) {
        reply(cmd.args_raw());
        return;
    }

    // /task start <name> <cron> [payload] | /task end <name>
    cmd = Command::parse(msg, "task", PrefixMode::Required('/'));
    if (!cmd.empty()) {
        cmd.on("start", [&reply](const std::vector<std::string>& args) {
            handle_task_start(reply, args);
        }).on("end", [&reply](const std::vector<std::string>& args) {
            handle_task_end(reply, args);
        });
    }
}

void handle_group_msg(uint64_t group_id, uint64_t _user_id, const std::string& msg) {
    auto reply = [group_id](const std::string& text) {
        luo9::bot::send_group_msg(group_id, text);
    };

    // /echo <text>
    auto cmd = Command::parse(msg, "echo", PrefixMode::Required('/'));
    if (!cmd.empty()) {
        reply(cmd.args_raw());
        return;
    }

    // /task start <name> <cron> [payload] | /task end <name>
    cmd = Command::parse(msg, "task", PrefixMode::Required('/'));
    if (!cmd.empty()) {
        cmd.on("start", [&reply](const std::vector<std::string>& args) {
            handle_task_start(reply, args);
        }).on("end", [&reply](const std::vector<std::string>& args) {
            handle_task_end(reply, args);
        });
    }
}

// Event handlers

void handle_meta_event(const luo9::payload::MetaEventPayload& ev) {
    switch (ev.meta_event_type) {
        case luo9::payload::MetaEventType::Heartbeat:
            // Heartbeat
            break;
        case luo9::payload::MetaEventType::Lifecycle:
            // Lifecycle
            break;
        default:
            break;
    }
}

void handle_notice(const luo9::payload::NoticePayload& notice) {
    switch (notice.notice_type) {
        case luo9::payload::NoticeType::GroupIncrease:
            // New member
            break;
        case luo9::payload::NoticeType::FriendAdd:
            // New friend
            break;
        default:
            break;
    }
}

void handle_task_event(const std::string& json) {
    // Simple JSON parsing for task event
    auto pos = json.find("\"task_name\"");
    if (pos == std::string::npos) return;
    
    auto start = json.find("\"", pos + 11);
    if (start == std::string::npos) return;
    start++;
    
    auto end = json.find("\"", start);
    if (end == std::string::npos) return;
    
    std::string task_name = json.substr(start, end - start);
    std::cout << "[task] Task triggered: name=" << task_name << std::endl;
}

// Task command handlers

void handle_task_start(const std::function<void(const std::string&)>& reply, const std::vector<std::string>& args) {
    // /task start <name> <sec min hour day month week> [payload]
    if (args.size() < 7) {
        reply("Usage: /task start <name> <sec min hour day month week> [payload]");
        return;
    }

    std::string name = args[0];
    std::string cron;
    for (int i = 1; i < 7; i++) {
        if (i > 1) cron += " ";
        cron += args[i];
    }
    
    std::string payload;
    if (args.size() > 7) {
        for (size_t i = 7; i < args.size(); i++) {
            if (i > 7) payload += " ";
            payload += args[i];
        }
    }

    std::string req = "{\"action\":\"schedule\",\"task_name\":\"" + name + "\",\"cron\":\"" + cron + "\",\"payload\":\"" + payload + "\"}";
    topic("luo9_task_miso").publish(req);
    reply("Task created: " + name + " [" + cron + "]");
}

void handle_task_end(const std::function<void(const std::string&)>& reply, const std::vector<std::string>& args) {
    // /task end <name>
    if (args.empty()) {
        reply("Usage: /task end <name>");
        return;
    }

    std::string name = args[0];
    std::string req = "{\"action\":\"cancel\",\"task_name\":\"" + name + "\"}";
    topic("luo9_task_miso").publish(req);
    reply("Task cancelled: " + name);
}
