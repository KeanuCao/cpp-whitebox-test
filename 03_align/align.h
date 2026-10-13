// ============================================================
//  知识点 3: 结构体内存对齐
//  同样 3 个成员, 换个顺序 sizeof 就从 12 变成 8
// ============================================================
#pragma once
#include <cstddef>

// (1) 原始顺序: char + int + char, 数据只有 6 字节, 却占 12 字节
struct Naive {
    char c;     // offset 0
    int  i;     // offset 4   <- 中间塞了 3 字节填充
    char c2;    // offset 8
};              // sizeof = 12 <- 尾部又塞 3 字节

// (2) 只换顺序, 成员一模一样, 变成 8 字节
struct Tuned {
    int  i;     // offset 0
    char c;     // offset 4
    char c2;    // offset 5
};              // sizeof = 8  <- 只填 2 字节

// (3) 对齐数 = min(成员自身大小, 编译器默认对齐数)
//     short 是 2 字节, 所以 char 后面只填 1 字节
struct Small {
    char  c1;   // offset 0
    short s;    // offset 2
    char  c2;   // offset 4
};              // sizeof = 6

// (4) 强制 1 字节对齐: 一点填充都不留, 但成员可能落在未对齐地址上
#pragma pack(push, 1)
struct Packed {
    char c;     // offset 0
    int  i;     // offset 1   <- 未对齐!
    char c2;    // offset 5
};              // sizeof = 6
#pragma pack(pop)
