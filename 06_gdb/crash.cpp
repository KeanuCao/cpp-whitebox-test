// ============================================================
//  知识点 6: 用 gdb 定位段错误
//  用法: ./build/bin/06_gdb [null|wild|oob|stack|deep]
//  练习: gdb ./build/bin/06_gdb  ->  run null  ->  bt
//
//  每个模式都配了一个"更深"的调用链, 这样 bt 才有东西可看
//
//  注意: 本文件编译时会故意产生 2 条警告
//    [-Wuninitialized]        mode_wild 里的 p 没初始化 —— 这正是要演示的
//    [-Winfinite-recursion]   recurse 无限递归 —— 这也是要演示的
//  这两条警告是教学材料, 不是错误, 不用修。
// ============================================================
#include <unistd.h>     // getpid()

#include <cstdio>
#include <cstdlib>
#include <cstring>

// ---------- 模式 null: 空指针解引用 ----------
static int read_from(int* p) {
    return *p;                 // p 是 nullptr 时, 崩在这一行
}
static int call_read_from(int* p) {
    return read_from(p);
}
static void mode_null() {
    std::printf("--- null: 对空指针解引用 ---\n");
    int* p = nullptr;
    std::printf("p = %p, 下一行就要读 *p\n", (void*)p);
    std::fflush(stdout);       // 先刷出来, 否则崩溃时看不到这行
    int v = call_read_from(p);
    std::printf("读到了 %d (永远到不了这里)\n", v);
}

// ---------- 模式 wild: 野指针写入 ----------
static void write_to(int* p) {
    *p = 12345;                // 往一个随机地址写
    std::printf("写完了 %d\n", *p);
}
static void mode_wild() {
    std::printf("--- wild: 未初始化的野指针 ---\n");
    int* p;                    // 未初始化! 里面是栈上的垃圾值
    // int* p = nullptr;       // <-- 动手改: 换成这行, 看崩溃地址的变化
    std::printf("p 的垃圾值 = %p\n", (void*)p);
    std::fflush(stdout);
    write_to(p);
}

// ---------- 模式 oob: 数组越界 (最阴的一种) ----------
static void mode_oob() {
    std::printf("--- oob: 数组越界写 ---\n");
    int arr[4] = {1, 2, 3, 4};
    int idx = 4;               // 合法下标是 0..3
    std::printf("arr 有 4 个元素, 现在写 arr[%d]\n", idx);
    std::fflush(stdout);
    arr[idx] = 999;            // 越界 1 个 int
    std::printf("居然没崩! arr[4] 的值 = %d\n", arr[idx]);
    std::printf("越界不一定立刻崩 —— 所以它比空指针更危险\n");
    std::printf("用 gdb 看: print &arr[4] 和 print &idx, 会发现地址重叠\n");
}

// ---------- 模式 stack: 无限递归 -> 栈溢出 ----------
static int recurse(int depth) {
    int pad[256];                          // 每层吃掉 1KB 栈
    std::memset(pad, depth & 0xFF, sizeof(pad));
    if (depth % 20000 == 0) {
        std::printf("  深度 %d\n", depth);
        std::fflush(stdout);
    }
    return recurse(depth + 1) + pad[0];
}
static void mode_stack() {
    std::printf("--- stack: 无限递归, 每层 1KB -> 8MB 栈很快见底 ---\n");
    std::fflush(stdout);
    std::printf("结果 = %d\n", recurse(1));
}

// ---------- 模式 deep: 正常的多层调用, 练习 bt / frame ----------
static int level3(int x) { return x * 2; }
static int level2(int x) { return level3(x) + 1; }
static int level1(int x) { return level2(x) - 3; }
static void mode_deep() {
    std::printf("--- deep: 不崩, 专门用来练习断点和 bt ---\n");
    std::printf("gdb 里: break level3  然后 run deep  然后 bt\n");
    std::printf("结果 = %d\n", level1(10));
}

int main(int argc, char** argv) {
    const char* mode = (argc > 1) ? argv[1] : "deep";
    std::printf("模式: %s  (pid=%d)\n\n", mode, (int)getpid());
    std::fflush(stdout);

    if      (std::strcmp(mode, "null")  == 0) mode_null();
    else if (std::strcmp(mode, "wild")  == 0) mode_wild();
    else if (std::strcmp(mode, "oob")   == 0) mode_oob();
    else if (std::strcmp(mode, "stack") == 0) mode_stack();
    else if (std::strcmp(mode, "deep")  == 0) mode_deep();
    else std::printf("未知模式。可选: null | wild | oob | stack | deep\n");

    std::printf("\n正常结束\n");
    return 0;
}
