// ============================================================
//  知识点 1: 指针的本质
//  一句话: 指针就是一个"存地址的整数", * 是按地址取货, & 是问地址。
//  跑法:   ./build/bin/01_pointer
// ============================================================
#include <cstdio>

int main() {
    // ---------- 1. 地址与解引用 ----------
    int  a = 42;
    int* p = &a;                       // p 存的是 a 的地址

    std::printf("=== 1. 地址与解引用 ===\n");
    std::printf("a  的值       = %d\n", a);
    std::printf("&a (a 的地址) = %p\n", (void*)&a);
    std::printf("p  的值       = %p   <- p 里装的就是 a 的地址\n", (void*)p);
    std::printf("*p (解引用)   = %d   <- 顺着地址去取货\n", *p);

    *p = 100;                          // 改 *p 就是改 a 本身
    std::printf("执行 *p = 100 后, a = %d\n\n", a);

    // ---------- 2. 指针自己占多大 ----------
    std::printf("=== 2. 指针本身的大小 ===\n");
    std::printf("sizeof(int)     = %zu\n", sizeof(int));
    std::printf("sizeof(char)    = %zu\n", sizeof(char));
    std::printf("sizeof(int*)    = %zu   <- 64 位系统上所有指针都是 8\n", sizeof(int*));
    std::printf("sizeof(char*)   = %zu   <- 和指向的类型无关\n", sizeof(char*));
    std::printf("sizeof(double*) = %zu   <- 还是 8\n\n", sizeof(double*));

    // ---------- 3. 指针算术 ----------
    std::printf("=== 3. 指针 +1 加的是 sizeof(T) 个字节 ===\n");
    int  arr[5] = {10, 20, 30, 40, 50};
    int* q = arr;                      // 数组名退化成首元素地址

    std::printf("q     = %p, *q     = %d\n", (void*)q, *q);
    std::printf("q + 1 = %p, *(q+1) = %d\n", (void*)(q + 1), *(q + 1));
    std::printf("两个地址差了 %ld 字节, 但只移动了 1 个元素\n",
                (long)((char*)(q + 1) - (char*)q));
    std::printf("arr[2] = %d, *(arr+2) = %d   <- 下标就是语法糖\n\n",
                arr[2], *(arr + 2));

    // ---------- 4. 二级指针 ----------
    std::printf("=== 4. 二级指针: 存“指针的地址” ===\n");
    int** pp = &p;                     // pp 指向 p, p 指向 a
    std::printf("a  的地址 = %p\n", (void*)&a);
    std::printf("p  的值   = %p   <- 和上面一样, 说明 p 指向 a\n", (void*)p);
    std::printf("pp 的值   = %p   <- 这是 p 自己的地址\n", (void*)pp);
    std::printf("*pp  = %p   <- 剥一层, 得到 p 的值\n", (void*)*pp);
    std::printf("**pp = %d   <- 剥两层, 得到 a\n", **pp);

    return 0;
}
