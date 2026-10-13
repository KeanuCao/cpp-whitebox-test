# ============================================================
#  解决 GoogleTest 依赖 —— 你不用管它从哪来
#
#  方案 A: 系统装了 libgtest-dev    -> 直接用, 最快
#  方案 B: 系统没装                 -> 自动从 GitHub 下载并编译(首次约 1 分钟)
#
#  两种方案最终都提供同一个目标:  GTest::gtest_main
#  所以子目录里只管写:
#      include(gtest)
#      target_link_libraries(你的测试 PRIVATE GTest::gtest_main)
# ============================================================

# 防止同一次 configure 里被重复 include。
# 判据是 target 是否已存在, 而不是自己记一个变量:
# CMake 每次 configure 都会重建所有 target, 所以这个判断天然不会跨 configure 残留。
# (踩过的坑: 用 set(... CACHE ...) 记标志位, 第二次 configure 会直接 return,
#  find_package 不再执行 -> GTest::gtest_main 消失 -> 整个工程配置失败)
if(TARGET GTest::gtest_main)
  return()
endif()

find_package(GTest QUIET)

if(GTest_FOUND)
  message(STATUS "[gtest] 方案A: 使用系统已安装的 GoogleTest")
else()
  message(STATUS "[gtest] 方案B: 系统没装, 自动下载 googletest (首次需要网络)")
  include(FetchContent)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(googletest)
endif()
