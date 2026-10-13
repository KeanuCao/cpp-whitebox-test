// ============================================================
//  知识点 5: 文件 I/O 与系统调用
//  跑法: ./build/bin/05_fileio          (在项目根目录跑)
//  偷看: strace -f -e trace=openat,write,read,close ./build/bin/05_fileio
// ============================================================
#include "fileio.h"

#include <cstdio>

int main() {
    const std::string text = "hello 白盒测试\n第二行\n";
    const char* f1 = "05_stream.txt";
    const char* f2 = "05_syscall.txt";

    std::printf("=== 1. 写文件: 两种方式干同一件事 ===\n");
    long n1 = write_text_stream(f1, text);
    long n2 = write_text_syscall(f2, text);
    std::printf("  fstream 写了 %ld 字节 -> %s\n", n1, f1);
    std::printf("  syscall 写了 %ld 字节 -> %s\n", n2, f2);

    std::printf("\n=== 2. 读回来对比 ===\n");
    std::string r1 = read_text_stream(f1);
    std::string r2 = read_text_syscall(f2);
    std::printf("  fstream 读回 %zu 字节\n", r1.size());
    std::printf("  syscall 读回 %zu 字节\n", r2.size());
    std::printf("  两者内容相同? %s\n", (r1 == r2) ? "是" : "否");
    std::printf("  和原文相同?   %s\n", (r1 == text) ? "是" : "否");

    std::printf("\n=== 3. 错误路径 (白盒测试最爱的分支) ===\n");
    std::printf("  读一个不存在的文件:\n");
    std::string missing = read_text_stream("根本没有这个文件.txt");
    std::printf("    fstream 返回空串, size = %zu\n", missing.size());
    std::string missing2 = read_text_syscall("根本没有这个文件.txt");
    std::printf("    syscall 返回空串, size = %zu\n", missing2.size());
    std::printf("  写一个不存在的目录:\n");
    long bad = write_text_syscall("/不存在的目录/x.txt", text);
    std::printf("    syscall 返回 %ld  (这就是 -1 分支)\n", bad);
    long bad2 = write_text_stream("/不存在的目录/x.txt", text);
    std::printf("    fstream 返回 %ld\n", bad2);

    std::printf("\n=== 4. 用 ls 确认文件真的在 ===\n");
    std::printf("  ls -l 05_*.txt\n");

    return 0;
}
