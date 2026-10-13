# gdb 定位段错误 —— 抄这一页就够

## 最短路径（背下来）

```bash
gdb ./build/bin/06_gdb
(gdb) run null          # 复现崩溃
(gdb) bt                # 看调用栈 —— 90% 的问题这一步就定位了
(gdb) frame 1           # 切到出事的栈帧（0 是库函数，1 才是你的代码）
(gdb) print p           # 看变量
(gdb) info locals       # 看这一帧所有局部变量
(gdb) quit
```

崩溃时 gdb 打印的那一行 `Program received signal SIGSEGV` 下面，
**第一行属于你自己代码的栈帧，就是凶手。**

## 常用命令

| 命令 | 作用 |
|---|---|
| `run [参数]` | 跑起来，`run null` 等于命令行传 `null` |
| `bt` / `backtrace` | 打印调用栈 |
| `frame N` | 切到第 N 层栈帧 |
| `print x` / `p x` | 打印变量（`p *p`、`p arr[4]`、`p/x p` 十六进制） |
| `info locals` | 当前帧全部局部变量 |
| `info args` | 当前帧函数参数 |
| `break file.cpp:42` | 在第 42 行下断点 |
| `break 函数名` | 在函数入口下断点 |
| `next` / `n` | 单步，不进函数 |
| `step` / `s` | 单步，进函数 |
| `continue` / `c` | 继续到下一个断点 |
| `list` / `l` | 看源码上下文 |
| `x/8xb p` | 以十六进制看 p 指向的 8 字节内存 |
| `watch x` | x 被改写时停下（找越界写的利器） |

## 五个模式分别练什么

| 模式 | 现象 | 重点看的 |
|---|---|---|
| `null` | 立刻 SIGSEGV | `bt` 看完整调用链，`p p` 是 `(int *) 0x0` |
| `wild` | 立刻 SIGSEGV，地址很怪 | `p p` 看那个垃圾地址，崩在 `write_to` |
| `oob` | **不崩**，但数据被改坏 | 用 `watch arr`，或用 `p &arr[4]` 对比 `p &idx` |
| `stack` | SIGSEGV，栈很深 | `bt` 会打印几千行，用 `bt 10` 只看前 10 层 |
| `deep` | 不崩 | 练 `break` + `bt` + `frame` |

## 三种查看崩溃的方式

```bash
# 1. 交互式（推荐初学者）
gdb ./build/bin/06_gdb

# 2. 一次性批处理：崩了就自动打印栈
gdb -batch -ex run -ex bt --args ./build/bin/06_gdb null

# 3. 事后验尸：先开 core dump
ulimit -c unlimited
./build/bin/06_gdb null          # 崩了，生成 core 文件
gdb ./build/bin/06_gdb core      # 直接看现场
```

## 和 valgrind 的分工

- **gdb**：程序**崩了**，我要知道崩在哪一行、怎么走到这的
- **valgrind**：程序**没崩**，但内存在悄悄泄漏/越界，我要它告诉我

```bash
valgrind --leak-check=full ./build/bin/06_gdb oob
```
