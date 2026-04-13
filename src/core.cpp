#include "main.h"

extern "C" void pmsg_process(uint64_t user_id, const char* msg) {
    if (msg == nullptr) return;
    
    std::string msg_copy(msg);
    handle_private_msg(user_id, msg_copy);
}

extern "C" void gmsg_process(uint64_t group_id, uint64_t user_id, const char* msg) {
    if (msg == nullptr) return;
    
    std::string msg_copy(msg);
    handle_group_msg(group_id, user_id, msg_copy);   
}

extern "C" void pevent_process(const char* json) {
}

extern "C" void gevent_process(const char* json) {
}
