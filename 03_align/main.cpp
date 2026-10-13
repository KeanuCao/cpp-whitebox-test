// ============================================================
//  知识点 3: 结构体与内存对齐
//  跑法: ./build/bin/03_align
// ============================================================
#include "align.h"

#include <cstdio>
#include <cstring>

int main() {
    // ---------- 1. 一样的成员, 顺序不同, sizeof 不同 ----------
    std::printf("=== 1. char+int+char, 数据 6 字节, sizeof 差 4 ===\n");

    std::printf("  Naive { char; int; char; }   sizeof = %zu\n", sizeof(Naive));
    std::printf("      offsetof(c)  = %zu\n", offsetof(Naive, c));
    std::printf("      offsetof(i)  = %zu   <- 中间 3 字节填充\n", offsetof(Naive, i));
    std::printf("      offsetof(c2) = %zu\n", offsetof(Naive, c2));

    std::printf("  Tuned { int; char; char; }   sizeof = %zu   <- 只是换了顺序\n", sizeof(Tuned));
    std::printf("      offsetof(i)  = %zu\n", offsetof(Tuned, i));
    std::printf("      offsetof(c)  = %zu\n", offsetof(Tuned, c));
    std::printf("      offsetof(c2) = %zu\n", offsetof(Tuned, c2));

    // ---------- 2. 对齐规则 ----------
    std::printf("\n=== 2. 对齐数 = min(成员大小, 默认对齐数) ===\n");
    std::printf("  Small { char; short; char; }  sizeof = %zu  <- short 对齐数是 2, 只填 1 字节\n",
                sizeof(Small));
    std::printf("      offsetof(s) = %zu\n", offsetof(Small, s));
    std::printf("  Packed  #pragma pack(1)       sizeof = %zu  <- 禁止填充\n", sizeof(Packed));
    std::printf("      offsetof(i) = %zu   <- int 落在未对齐地址上!\n", offsetof(Packed, i));

    // ---------- 3. 填充字节里到底是什么? ----------
    std::printf("\n=== 3. 填充字节里装的是什么? ===\n");
    Naive n;
    std::memset(&n, 0xAB, sizeof(n));   // 先把整块内存填成 0xAB
    n.c  = 1;                           // 只写真实成员
    n.i  = 2;
    n.c2 = 3;

    const unsigned char* raw = reinterpret_cast<const unsigned char*>(&n);
    std::printf("  逐字节(十六进制): ");
    for (std::size_t k = 0; k < sizeof(n); ++k) std::printf("%02X ", raw[k]);
    std::printf("\n  出现 0xAB 的位置就是填充字节 —— 它们不存数据, 也没人帮你清零\n");
    std::printf("  所以: 用 memcmp 直接比较两个结构体是错的, 填充字节可能是垃圾\n");

    return 0;
}
