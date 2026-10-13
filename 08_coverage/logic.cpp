#include "logic.h"

namespace logic {

// 【坑 1】&& 短路
bool can_drive(int age, bool has_license) {
    if (age >= 18 && has_license) {   // 两个条件, 最多 4 条分支
        return true;
    }
    return false;
}

// 【坑 2】多路 if-else
int classify(int score) {
    if (score < 0)        return -1;
    else if (score < 60)  return 0;
    else if (score < 90)  return 1;
    else                  return 2;
}

int square(int x) {
    return x * x;
}

}  // namespace logic
