#include "main.h"

using namespace luo9::bus;
using namespace luo9::payload;
using namespace luo9::version;

extern "C" void plugin_main() {
    // Subscribe to topics
    auto msg_sub = topic("luo9_message").subscribe();
    auto event_sub = topic("luo9_meta_event").subscribe();
    auto notice_sub = topic("luo9_notice").subscribe();
    auto task_sub = topic("luo9_task").subscribe();
    auto ver_sub = topic("luo9_version").subscribe();

    // Get topic references
    auto msg_topic = topic("luo9_message");
    auto event_topic = topic("luo9_meta_event");
    auto notice_topic = topic("luo9_notice");
    auto task_topic = topic("luo9_task");
    auto ver_topic = topic("luo9_version");

    while (true) {
        // Messages
        auto json = msg_topic.pop(msg_sub);
        if (!json.empty()) {
            auto payload = BusPayload::parse(json);
            if (payload.type == PayloadType::Message) {
                switch (payload.message.message_type) {
                    case MsgType::Group:
                        handle_group_msg(
                            payload.message.group_id.value_or(0),
                            payload.message.user_id,
                            payload.message.message
                        );
                        break;
                    case MsgType::Private:
                        handle_private_msg(payload.message.user_id, payload.message.message);
                        break;
                    default:
                        break;
                }
            }
        }

        // Meta events
        json = event_topic.pop(event_sub);
        if (!json.empty()) {
            auto payload = BusPayload::parse(json);
            if (payload.type == PayloadType::MetaEvent) {
                handle_meta_event(payload.meta_event);
            }
        }

        // Notices
        json = notice_topic.pop(notice_sub);
        if (!json.empty()) {
            auto payload = BusPayload::parse(json);
            if (payload.type == PayloadType::Notice) {
                handle_notice(payload.notice);
            }
        }

        // Task events
        json = task_topic.pop(task_sub);
        if (!json.empty()) {
            handle_task_event(json);
        }

        // Version query
        json = ver_topic.pop(ver_sub);
        if (!json.empty()) {
            if (is_version_query(json)) {
                reply_version("plugin_echo", "0.1.0");
            }
        }

        // Yield CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
