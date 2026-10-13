#include "list.h"

#include <cstdio>

List::~List() {
    Node* cur = head_;
    while (cur) {
        Node* next = cur->next;   // 先存下一个, 再删当前
        delete cur;
        cur = next;
    }
}

void List::push_front(int v) {
    Node* n = new Node(v);
    n->next = head_;
    head_    = n;
    ++size_;
}

void List::push_back(int v) {
    Node* n = new Node(v);
    if (!head_) {
        head_ = n;
    } else {
        Node* cur = head_;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    ++size_;
}

// 用二级指针删节点: 头结点不用特判, 这是知识点 1 的直接应用
bool List::remove(int v) {
    Node** pp = &head_;
    while (*pp) {
        if ((*pp)->value == v) {
            Node* dead = *pp;
            *pp        = dead->next;
            delete dead;
            --size_;
            return true;
        }
        pp = &(*pp)->next;
    }
    return false;
}

// 三个指针倒方向, 画一遍图就懂了
void List::reverse() {
    Node* prev = nullptr;
    Node* cur  = head_;
    while (cur) {
        Node* next = cur->next;   // 1. 记住后面
        cur->next  = prev;        // 2. 把箭头掉头
        prev       = cur;         // 3. prev 前进
        cur        = next;        // 4. cur 前进
    }
    head_ = prev;
}

int List::at(std::size_t idx) const {
    if (idx >= size_) return -1;   // 边界: 空链表、越界都走这里
    const Node* cur = head_;
    for (std::size_t k = 0; k < idx; ++k) cur = cur->next;
    return cur->value;
}

void List::print(const char* tag) const {
    std::printf("  %-10s [", tag);
    for (const Node* cur = head_; cur; cur = cur->next) {
        std::printf("%d", cur->value);
        if (cur->next) std::printf(" -> ");
    }
    std::printf("]  size=%zu\n", size_);
}
