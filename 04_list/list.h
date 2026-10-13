// ============================================================
//  知识点 4: 手写单链表 (含反转)
//  逻辑和 main 分开: 这个文件 + list.cpp 是"被测代码"
// ============================================================
#pragma once
#include <cstddef>

struct Node {
    int   value;
    Node* next;
    explicit Node(int v) : value(v), next(nullptr) {}
};

class List {
public:
    List() : head_(nullptr), size_(0) {}
    ~List();                                   // 析构要释放所有节点, 否则整条链泄漏

    // 禁止拷贝: 默认拷贝是浅拷贝, 两个 List 指向同一批节点 -> double free
    List(const List&)            = delete;
    List& operator=(const List&) = delete;

    void        push_front(int v);             // 头插 O(1)
    void        push_back(int v);              // 尾插 O(n)
    bool        remove(int v);                 // 删除第一个值为 v 的节点
    void        reverse();                     // 原地反转
    int         at(std::size_t idx) const;     // 越界返回 -1 (可测的边界)
    std::size_t size()  const { return size_; }
    bool        empty() const { return head_ == nullptr; }
    void        print(const char* tag) const;

private:
    Node*       head_;
    std::size_t size_;
};
