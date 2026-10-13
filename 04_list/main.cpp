// ============================================================
//  知识点 4: 手写单链表 (含反转)
//  跑法: ./build/bin/04_list
//  必做: valgrind --leak-check=full ./build/bin/04_list
// ============================================================
#include "list.h"

#include <cstdio>

int main() {
    List l;

    std::printf("=== 1. 头插 / 尾插 ===\n");
    l.print("空链表");
    l.push_back(3);
    l.push_front(2);
    l.push_front(1);
    l.push_back(4);
    l.print("插完");                    // 期望 1 -> 2 -> 3 -> 4

    std::printf("\n=== 2. 反转 ===\n");
    l.print("反转前");
    l.reverse();
    l.print("反转后");                  // 期望 4 -> 3 -> 2 -> 1
    l.reverse();
    l.print("再反转");                  // 转回来, 还是 1 -> 2 -> 3 -> 4

    std::printf("\n=== 3. 按值删除 ===\n");
    std::printf("  remove(2) 返回 %d\n", (int)l.remove(2));
    l.print("删 2 后");
    std::printf("  remove(1) 返回 %d   <- 删头结点, 二级指针在这里救场\n", (int)l.remove(1));
    l.print("删 1 后");
    std::printf("  remove(99) 返回 %d  <- 值不存在, 返回 false\n", (int)l.remove(99));

    std::printf("\n=== 4. 边界情况 ===\n");
    std::printf("  at(0)  = %d\n", l.at(0));
    std::printf("  at(1)  = %d\n", l.at(1));
    std::printf("  at(2)  = %d   <- 越界, 返回 -1\n", l.at(2));
    std::printf("  at(99) = %d  <- 远距离越界, 还是 -1, 不会崩\n", l.at(99));

    List empty;
    std::printf("\n  空链表试一下:\n");
    empty.print("空链表");
    empty.reverse();
    empty.print("空表反转");            // 不能崩
    std::printf("  empty.at(0) = %d\n", empty.at(0));
    std::printf("  empty.remove(1) = %d\n", (int)empty.remove(1));

    std::printf("\n=== 5. 析构 ===\n");
    std::printf("  main 结束时 l 析构, 4 个节点全部 delete, valgrind 应该报 0 leaks\n");

    return 0;
}
