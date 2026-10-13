# ============================================================
#  C++ 底层开发 + 白盒测试   Basic 阶段 (8 个知识点)
#
#  一键编译:  cmake -S . -B build && cmake --build build -j
#  产物位置:  build/bin/  名字就是知识点编号, 如 build/bin/01_pointer
#  跑测试:    cd build && ctest --output-on-failure
#  看覆盖率:  bash scripts/coverage.sh
# ============================================================
cmake_minimum_required(VERSION 3.16)
project(cpp_whitebox_test LANGUAGES CXX)

# ---- 统一 C++ 标准, 避免各知识点行为不一致 ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 默认 Debug: gdb 要行号、valgrind 要行号, 都靠 -g ----
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---- 所有可执行文件统一放到 build/bin/ ----
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- 打开警告: 白盒测试的第一步是先看见编译器的抱怨 ----
add_compile_options(-Wall -Wextra)

# ---- 覆盖率开关(第 8 个知识点用) ----
#    cmake -S . -B build-cov -DENABLE_COVERAGE=ON
option(ENABLE_COVERAGE "开启 gcov 覆盖率编译选项" OFF)
if(ENABLE_COVERAGE)
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
  message(STATUS "覆盖率已开启: --coverage")
endif()

# ---- 让子目录里的 include(gtest) 能找到 cmake/gtest.cmake ----
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ---- 注册测试, 必须放在 add_subdirectory 之前 ----
enable_testing()

# ---- 解析 GoogleTest 依赖 ----
# 必须在这里(顶层)include 一次: CMake 的 imported target 是"目录作用域"的,
# 在 07_gtest 里 include 出来的 GTest::gtest_main, 兄弟目录 08_coverage 看不见。
include(gtest)

# ---- 8 个知识点, 各占一个子目录 ----
add_subdirectory(01_pointer)   # 指针的本质
add_subdirectory(02_memory)    # malloc/free 与 new/delete
add_subdirectory(03_align)     # 结构体与内存对齐
add_subdirectory(04_list)      # 手写单链表(含反转)
add_subdirectory(05_fileio)    # 文件 I/O 与系统调用
add_subdirectory(06_gdb)       # 用 gdb 定位段错误
add_subdirectory(07_gtest)     # GoogleTest 白盒测试入门
add_subdirectory(08_coverage)  # 代码覆盖率 (gcov + lcov)
add_subdirectory(09_realworld) # 实战: 给真实工程代码写白盒测试
