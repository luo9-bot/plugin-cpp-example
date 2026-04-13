#ifndef __CORE_H__
#define __CORE_H__

#include <cstdint>

void handle_private_msg(uint64_t user_id, const std::string& msg);

void handle_group_msg(uint64_t group_id, uint64_t user_id, const std::string& msg);

#endif // __CORE_H__