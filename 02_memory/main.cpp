// ============================================================
//  知识点 2: 动态内存  malloc/free  与  new/delete
//  用法: ./build/bin/02_memory [clean|leak|mismatch|malloc_obj|new_delete]
//  必做: valgrind --leak-check=full ./build/bin/02_memory leak
//
//  注意: 本文件编译时会产生 2 条警告, 都是故意埋的教学材料
//    [-Wmismatched-new-delete]  demo_mismatch 里 new[] 配了 delete
//    [-Wuninitialized]          demo_malloc_obj 里读了未初始化的成员
//  另外 mismatch 模式是未定义行为, 可能直接 abort(退出码 134), 这是预期现象。
// ============================================================
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ---------- 一个会"说话"的类: 用来观察构造/析构到底有没有被调用 ----------
struct Tracked {
    int id;
    explicit Tracked(int i) : id(i) { std::printf("      [构造] Tracked(%d)\n", id); }
    ~Tracked()                      { std::printf("      [析构] ~Tracked(%d)\n", id); }
};

// ---------- 1. 正确配对 ----------
static void demo_clean() {
    std::printf("--- clean: malloc / free 正确配对 ---\n");
    int* p = static_cast<int*>(std::malloc(sizeof(int) * 4));
    if (!p) { std::printf("      malloc 失败\n"); return; }

    for (int i = 0; i < 4; ++i) p[i] = i * i;
    std::printf("      4 个 int: %d %d %d %d\n", p[0], p[1], p[2], p[3]);
    std::free(p);
    std::printf("      已 free, valgrind 不会报错\n");
}

// ---------- 2. 内存泄漏 ----------
static void demo_leak() {
    std::printf("--- leak: 分配了不释放 ---\n");
    int* p = static_cast<int*>(std::malloc(sizeof(int) * 100));
    if (!p) return;
    p[0] = 1;
    std::printf("      malloc 了 400 字节, 函数一返回 p 就消失\n");
    std::printf("      这 400 字节再也没有任何指针指向它 -> 泄漏\n");
    // std::free(p);   // <-- 故意注释掉, 这就是泄漏
}

// ---------- 3. new[] 配 delete (不是 delete[]) ----------
static void demo_mismatch() {
    std::printf("--- mismatch: new[] 却用 delete 释放 ---\n");
    Tracked* arr = new Tracked[3]{Tracked(1), Tracked(2), Tracked(3)};

    // 注意: 下面这句 delete 是未定义行为, 可能导致程序直接 abort,
    //       所以所有说明必须写在它【前面】, 并且手动 fflush 刷出去。
    std::printf("      3 个元素都构造了。正确写法是  delete[] arr;\n");
    std::printf("      下面是错的  delete arr;  —— 盯住 [析构] 到底出现几次\n\n");
    std::printf("      未定义行为会有两种表现, 都算正常:\n");
    std::printf("        a) 只析构 1 次, 然后 munmap_chunk(): invalid pointer -> abort\n");
    std::printf("        b) 只析构 1 次, 程序看着没事, 但堆已经坏了\n");
    std::printf("      (直接跑通常是 a; 套一层 valgrind 跑通常是 b,\n");
    std::printf("       因为 valgrind 换掉了 malloc/free, 于是它有机会报:\n");
    std::printf("       Invalid free() / delete / delete[])\n\n");
    std::printf("      >>> 现在执行错误的 delete:\n");
    std::fflush(stdout);

    delete arr;        // <-- 错误! 应该是 delete[] arr;  这一行可能直接终止程序

    std::printf("\n      (这次是情况 b, 没崩 —— 但别高兴, 换个编译器它就 abort 了)\n");
}

// ---------- 4. 用 malloc 创建 C++ 对象 ----------
static void demo_malloc_obj() {
    std::printf("--- malloc_obj: 用 malloc 造 C++ 对象 ---\n");
    Tracked* t = static_cast<Tracked*>(std::malloc(sizeof(Tracked)));
    std::printf("      上面没有 [构造] 输出 -> 构造函数根本没被调用\n");
    std::printf("      t->id 是未初始化的垃圾值: %d\n", t->id);
    std::free(t);
    std::printf("      free 也没有 [析构] 输出 -> 用 malloc 造 C++ 对象就是错的\n");
}

// ---------- 5. 正确姿势 ----------
static void demo_new_delete() {
    std::printf("--- new_delete: 正确配对, 构造/析构都调用 ---\n");
    std::printf("   单个对象:\n");
    Tracked* one = new Tracked(7);
    std::printf("      one->id = %d\n", one->id);
    delete one;

    std::printf("   对象数组:\n");
    Tracked* arr = new Tracked[2]{Tracked(8), Tracked(9)};
    delete[] arr;      // <-- 数组一定要带 []
    std::printf("      构造 2 次、析构 2 次, 一一对应\n");
}

int main(int argc, char** argv) {
    const char* mode = (argc > 1) ? argv[1] : "new_delete";
    std::printf("模式: %s\n\n", mode);

    if      (std::strcmp(mode, "clean")      == 0) demo_clean();
    else if (std::strcmp(mode, "leak")       == 0) demo_leak();
    else if (std::strcmp(mode, "mismatch")   == 0) demo_mismatch();
    else if (std::strcmp(mode, "malloc_obj") == 0) demo_malloc_obj();
    else if (std::strcmp(mode, "new_delete") == 0) demo_new_delete();
    else std::printf("未知模式。可选: clean | leak | mismatch | malloc_obj | new_delete\n");

    return 0;
}
