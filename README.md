# C++ 底层开发 + 白盒测试 —— Basic 阶段

8 个知识点，每个都是**能跑的程序**，不是讲义。
学法是：**改一行 → 重编 → 看变化**。

---

## 一、环境（已经装好了）

WSL 发行版 **`cpp-test`**（Ubuntu 24.04）。已装：

| 工具 | 用途 | 用在哪一关 |
|---|---|---|
| g++ 13.3 / cmake / make | 编译 | 全部 |
| gdb | 定位段错误 | 第 6 关 |
| valgrind | 查内存泄漏/越界 | 第 2 关 |
| libgtest-dev | 单元测试框架 | 第 7、8 关 |
| lcov / genhtml | 覆盖率报告 | 第 8 关 |

打开 WSL（开始菜单里选 `cpp-test`，不要选 Ubuntu-22.04），然后：

```bash
cd /mnt/c/wp/cpp-whitebox-test
```

> 仓库放在 Windows 盘上，WSL 里通过 `/mnt/c/...` 访问。
> 在 Windows 侧直接跑 `cmake` 是不行的，工具装在 WSL 里。

---

## 二、一键编译 + 跑测试

```bash
cmake -S . -B build          # 配置（只需第一次，或改了 CMakeLists.txt 之后）
cmake --build build -j       # 编译全部 8 关
cd build && ctest --output-on-failure   # 跑全部 23 个测试用例
```

所有可执行文件都在 **`build/bin/`** 下，名字就是关卡编号：

```
build/bin/01_pointer      build/bin/05_fileio
build/bin/02_memory       build/bin/06_gdb
build/bin/03_align        build/bin/test_calc
build/bin/04_list         build/bin/test_logic
```

---

## 三、八关怎么跑

| # | 关卡 | 命令 | 必须动手试的 |
|---|---|---|---|
| 1 | 指针的本质 | `./build/bin/01_pointer` | 把 `int* p` 改成 `char* p`，看 `sizeof` 和输出怎么变 |
| 2 | 内存管理 | `./build/bin/02_memory leak` | 加 `valgrind --leak-check=full` 前缀，看它怎么指出泄漏行号 |
| 3 | 内存对齐 | `./build/bin/03_align` | 把 `Naive` 的成员顺序换成 `Tuned` 的顺序，看 `sizeof` 从 12 掉到 8 |
| 4 | 单链表 | `./build/bin/04_list` | 把 `reverse()` 里的 4 行顺序打乱，看链表怎么断 |
| 5 | 文件 I/O | `./build/bin/05_fileio` | 用 `strace -e trace=openat,write,read` 跑，看 fstream 底下是什么 |
| 6 | gdb 调试 | `./build/bin/06_gdb null` | 用 gdb 跑，`bt` 看调用栈（详见 `06_gdb/GDB_CHEATSHEET.md`） |
| 7 | GoogleTest | `cd build && ctest -R IsPrime` | 打开 `07_gtest/test_calc.cpp`，把一个 `EXPECT_EQ` 的期望值改错，看报错长什么样 |
| 8 | 覆盖率 | `bash scripts/coverage.sh` | 见下方"第 8 关" |

### 第 2 关：valgrind 三种玩法

```bash
./build/bin/02_memory new_delete             # 正确姿势：构造/析构一一对应
valgrind --leak-check=full ./build/bin/02_memory leak      # 内存泄漏
./build/bin/02_memory mismatch               # new[] 配 delete
valgrind ./build/bin/02_memory mismatch      # 同一个错误，valgrind 视角
valgrind ./build/bin/02_memory malloc_obj    # malloc 造 C++ 对象
```

> `mismatch` 是未定义行为，**直接跑通常会 abort**（`munmap_chunk(): invalid pointer`，退出码 134）。
> 套一层 valgrind 跑反而不崩，会打印 `Invalid free() / delete / delete[]`。
> 两种都是"正确"的 UB 表现 —— 这正是 UB 可怕的地方。

### 第 6 关：gdb

```bash
./build/bin/06_gdb          # 列出所有模式
gdb ./build/bin/06_gdb
(gdb) run null              # 复现崩溃
(gdb) bt                    # 看调用栈 ← 90% 的问题这一步就定位了
```

五种模式：`null`（空指针）/ `wild`（野指针）/ `oob`（越界，**不崩但数据被改坏**）/
`stack`（递归爆栈）/ `deep`（不崩，专门练断点）。
完整命令表见 **`06_gdb/GDB_CHEATSHEET.md`**。

### 第 8 关：覆盖率（这一关最关键）

```bash
bash scripts/coverage.sh
# 然后浏览器打开  build-cov/html/index.html
```

跑完你会看到：

```
  07_gtest/calc.cpp          行  21/21   100.0%   分支  18/20    90.0%
  08_coverage/logic.cpp      行  11/11   100.0%   分支   9/10    90.0%
```

**行覆盖 100%，分支覆盖却只有 90%。** 少的那条分支藏在
`08_coverage/logic.cpp` 的 `can_drive()` 里：

```cpp
if (age >= 18 && has_license) {   // && 短路
```

现有测试只喂了 `(20,true)` 和 `(15,true)` —— 第二种情况下 `age >= 18` 为假，
`has_license` **根本没被求值**，所以"它为假"那条分支从没走过。

`&&` / `||` 的短路特性，正是"行覆盖 100% 但分支覆盖不满"最常见的成因。

**动手改**：打开 `08_coverage/test_logic.cpp`，把这段的注释取消掉，再跑一次：

```cpp
// TEST(CanDrive, AdultWithoutLicense) {
//     EXPECT_FALSE(logic::can_drive(20, false));
// }
```

分支覆盖应该从 90% 涨到 **100%**。（已实测：`9/10` → `10/10`）

---

## 三点五、第 9 关：真实工程代码（实战）

`09_realworld/` 里是从真实项目（半导体测试框架）拷出来的 `runfunctionjob.cpp`。
这个模块的玩法不一样 —— **代码是给定的，你只管测**：

```bash
./build/bin/test_runfunctionjob                        # 20 个用例
cd build && ctest -R RunFunctionJob                    # 只跑这个模块
```

`RunFunctionJob::run()` 有 **7 条出口路径**（1 个 return false + 5 个 break + 1 个正常返回），
`test_runfunctionjob.cpp` 里每个 TEST 对应一条。读完那个文件的注释，你会看到白盒测试的完整闭环：

```
读代码画控制流 → 写测试覆盖每条路径 → 看覆盖率报告 → 发现漏洞 → 补测试
```

两个只有真实工程才会教你的结论：

1. **分支覆盖率到不了 100% 是正常的。** `runfunctionjob.cpp` 行覆盖 100%，分支只有 57% ——
   差的 43 条全是 `(throw)`，编译器给 `shared_ptr`/`vector` 生成的异常路径，实践中无法触发。
2. **用替身换来的可测性有代价。** 测试全程用替身，结果 `RunSuite`/`FakeRunSuite`
   这两个生产实现一行都没跑到（覆盖率报告里 `runsuite.h` 只有 3/11 行）。补两个用例才修好。

## 四、目录结构

```
cpp-whitebox-test/
├── CMakeLists.txt          # 顶层：统一 C++17、Debug、-Wall -Wextra、覆盖率开关
├── cmake/gtest.cmake       # gtest 依赖：系统装了就用系统的，没装自动下载
├── scripts/coverage.sh     # 一键覆盖率
├── 01_pointer/  ... 06_gdb/          # 每关一个子目录
├── 07_gtest/    calc.{h,cpp} + test_calc.cpp
├── 08_coverage/ logic.{h,cpp} + test_logic.cpp
└── 09_realworld/ runfunctionjob.{h,cpp}   ← 从真实项目拷来的
                   + 5 个补齐依赖的 .h + test_runfunctionjob.cpp
```

第 4 关之后，**业务逻辑都拆成了独立的静态库**（`list_lib`、`calc_lib`、`logic_lib`…），
`main` 只负责演示。所以测试能直接链接逻辑、覆盖率能统计到逻辑 —— 而不是只测到 `main`。

自己加新练习时请保持这个结构。

---

## 五、常见问题

**`cmake: command not found`**
你开错发行版了。用 `cpp-test`，不是 `Ubuntu-22.04`。

**编译时出现 4 条警告**
全是故意的，都是教学材料，不用修：

| 文件 | 警告 | 对应哪一关 |
|---|---|---|
| `02_memory/main.cpp` | `-Wmismatched-new-delete` | `demo_mismatch`：new[] 配了 delete |
| `02_memory/main.cpp` | `-Wuninitialized` | `demo_malloc_obj`：读了未初始化的成员 |
| `06_gdb/crash.cpp` | `-Wuninitialized` | `mode_wild`：野指针 |
| `06_gdb/crash.cpp` | `-Winfinite-recursion` | `mode_stack`：递归爆栈 |

顺带一提：编译器在你运行之前就已经警告了，这本身就是个知识点。

**改了代码没生效**
`cmake --build build` 要重新跑一遍。改过 `CMakeLists.txt` 则要先 `cmake -S . -B build` 重新配置。

**覆盖率报 `branches: no data found`**
说明 `--coverage` 没生效。用 `scripts/coverage.sh`，它会自己带 `-DENABLE_COVERAGE=ON`；
手动配置的话记得加上这个开关。

**编译时刷 `gmake: Warning: Clock skew detected. Your build may be incomplete.`**
无害，不用管。仓库放在 `/mnt/c`（Windows 盘）上，从 Windows 侧改过文件后，
drvfs 报出来的时间戳会短暂"快"零点几秒。**再编一两次就自己消失了**（实测 3 条 → 2 条 → 0 条）。
