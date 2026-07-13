#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

#include "luo9_sdk.h"

// Message handlers
void handle_private_msg(uint64_t user_id, const std::string& msg);
void handle_group_msg(uint64_t group_id, uint64_t user_id, const std::string& msg);

// Event handlers
void handle_meta_event(const luo9::payload::MetaEventPayload& ev);
void handle_notice(const luo9::payload::NoticePayload& notice);
void handle_task_event(const std::string& json);

// Task command handlers
void handle_task_start(const std::function<void(const std::string&)>& reply, const std::vector<std::string>& args);
void handle_task_end(const std::function<void(const std::string&)>& reply, const std::vector<std::string>& args);

#endif // __MAIN_H__
