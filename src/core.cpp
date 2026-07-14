#include "main.h"
#include <windows.h>

#ifdef _WIN32
#define DEBUG_LOG(msg) do { \
    OutputDebugStringA(msg); \
    fprintf(stderr, "%s\n", msg); \
    fflush(stderr); \
} while(0)
#else
#define DEBUG_LOG(msg) do { \
    fprintf(stderr, "%s\n", msg); \
    fflush(stderr); \
} while(0)
#endif

using namespace luo9::bus;
using namespace luo9::payload;
using namespace luo9::version;

extern "C" void plugin_main() {
    DEBUG_LOG("[plugin_echo] plugin_main() started");

    // Subscribe to topics
    DEBUG_LOG("[plugin_echo] Subscribing to topics...");
    
    auto msg_sub = topic("luo9_message").subscribe();
    DEBUG_LOG(("[plugin_echo] luo9_message sub_id=" + std::to_string(msg_sub)).c_str());
    
    auto event_sub = topic("luo9_meta_event").subscribe();
    DEBUG_LOG(("[plugin_echo] luo9_meta_event sub_id=" + std::to_string(event_sub)).c_str());
    
    auto notice_sub = topic("luo9_notice").subscribe();
    DEBUG_LOG(("[plugin_echo] luo9_notice sub_id=" + std::to_string(notice_sub)).c_str());
    
    auto task_sub = topic("luo9_task").subscribe();
    DEBUG_LOG(("[plugin_echo] luo9_task sub_id=" + std::to_string(task_sub)).c_str());
    
    auto ver_sub = topic("luo9_version").subscribe();
    DEBUG_LOG(("[plugin_echo] luo9_version sub_id=" + std::to_string(ver_sub)).c_str());

    // Get topic references
    auto msg_topic = topic("luo9_message");
    auto event_topic = topic("luo9_meta_event");
    auto notice_topic = topic("luo9_notice");
    auto task_topic = topic("luo9_task");
    auto ver_topic = topic("luo9_version");

    DEBUG_LOG("[plugin_echo] Entering main loop...");

    while (true) {
        // Messages
        auto json = msg_topic.pop(msg_sub);
        if (!json.empty()) {
            DEBUG_LOG(("[plugin_echo] Received message: " + json.substr(0, 200)).c_str());
            auto payload = BusPayload::parse(json);
            DEBUG_LOG(("[plugin_echo] Parsed payload type: " + std::to_string(static_cast<int>(payload.type))).c_str());
            if (payload.type == PayloadType::Message) {
                DEBUG_LOG(("[plugin_echo] Message type: " + std::to_string(static_cast<int>(payload.message.message_type))).c_str());
                DEBUG_LOG(("[plugin_echo] Message content: " + payload.message.message).c_str());
                DEBUG_LOG(("[plugin_echo] Message sub_type: " + payload.message.sub_type).c_str());
                switch (payload.message.message_type) {
                    case MsgType::Group:
                        DEBUG_LOG(("[plugin_echo] Group msg from user=" + std::to_string(payload.message.user_id) + " group=" + std::to_string(payload.message.group_id.value_or(0))).c_str());
                        handle_group_msg(
                            payload.message.group_id.value_or(0),
                            payload.message.user_id,
                            payload.message.message
                        );
                        break;
                    case MsgType::Private:
                        DEBUG_LOG(("[plugin_echo] Private msg from user=" + std::to_string(payload.message.user_id)).c_str());
                        handle_private_msg(payload.message.user_id, payload.message.message);
                        break;
                    default:
                        DEBUG_LOG("[plugin_echo] Unknown message type");
                        break;
                }
            } else {
                DEBUG_LOG(("[plugin_echo] Payload type: " + std::to_string(static_cast<int>(payload.type))).c_str());
            }
        }

        // Meta events
        json = event_topic.pop(event_sub);
        if (!json.empty()) {
            DEBUG_LOG(("[plugin_echo] Received meta_event: " + json).c_str());
            auto payload = BusPayload::parse(json);
            if (payload.type == PayloadType::MetaEvent) {
                handle_meta_event(payload.meta_event);
            }
        }

        // Notices
        json = notice_topic.pop(notice_sub);
        if (!json.empty()) {
            DEBUG_LOG(("[plugin_echo] Received notice: " + json).c_str());
            auto payload = BusPayload::parse(json);
            if (payload.type == PayloadType::Notice) {
                handle_notice(payload.notice);
            }
        }

        // Task events
        json = task_topic.pop(task_sub);
        if (!json.empty()) {
            DEBUG_LOG(("[plugin_echo] Received task event: " + json).c_str());
            handle_task_event(json);
        }

        // Version query
        json = ver_topic.pop(ver_sub);
        if (!json.empty()) {
            DEBUG_LOG(("[plugin_echo] Received version query: " + json).c_str());
            if (is_version_query(json)) {
                DEBUG_LOG("[plugin_echo] Responding to version query");
                reply_version("plugin_echo", "0.1.0");
            }
        }

        // Yield CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
