// ============================================================
//  知识点 5: 文件 I/O —— C++ 流  vs  Linux 系统调用
//  两组函数干同一件事, 对比着看
// ============================================================
#pragma once
#include <string>

// ---- 方式一: C++ 流 (fstream), 内部最终还是调用系统调用 ----
// 返回写入的字节数, 失败返回 -1
long write_text_stream(const std::string& path, const std::string& text);
// 读整个文件, 失败返回空串
std::string read_text_stream(const std::string& path);

// ---- 方式二: Linux 系统调用 open/write/read/close ----
// 返回写入的字节数, 失败返回 -1
long write_text_syscall(const std::string& path, const std::string& text);
// 读整个文件, 失败返回空串
std::string read_text_syscall(const std::string& path);
