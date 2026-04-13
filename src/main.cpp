#include "main.h"

void handle_private_msg(uint64_t user_id, const std::string& msg) {
    auto cmd = Command::parse(msg, "echo", PrefixMode::Optional('/'));
    if (!cmd.empty()) {
        Bot::send_private_msg(user_id, cmd.args_raw());
    }
}

void handle_group_msg(uint64_t group_id, uint64_t user_id, const std::string& msg) {
    auto cmd = Command::parse(msg, "echo", PrefixMode::Optional('/'));
    if (!cmd.empty()) {
        Bot::send_group_msg(group_id, cmd.args_raw());
    }
}