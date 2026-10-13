#!/usr/bin/env bash
# ============================================================
#  第 8 个知识点: 一键覆盖率
#  跑法: 在项目根目录  bash scripts/coverage.sh
#  产物: build-cov/html/index.html   (用浏览器打开)
# ============================================================
set -euo pipefail
cd "$(dirname "$0")/.."

# lcov 2.0 的坑 1: --ignore-errors 要把类别名写两遍, 才能同时压掉 error 和 warning
# lcov 2.0 的坑 2: genhtml 不认 'gcov' 这个类别, 所以两个阶段要分开写
IGNORE_LCOV="mismatch,mismatch,gcov,gcov,source,source,empty,empty,unused,unused"
IGNORE_HTML="mismatch,mismatch,source,source,empty,empty,unused,unused"

echo "==> 1/4 用 --coverage 重新配置并编译"
cmake -S . -B build-cov -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug >/dev/null
# 编译日志落盘: 失败时看日志, 成功时不刷屏, 且不会被 grep 吞掉退出码
if ! cmake --build build-cov -j"$(nproc)" > build-cov/build.log 2>&1; then
    echo "!! 编译失败, build-cov/build.log 最后 30 行:"
    tail -30 build-cov/build.log
    exit 1
fi

echo "==> 2/4 跑全部测试 (覆盖率数据在这一步产生)"
( cd build-cov && ctest --output-on-failure )

echo "==> 3/4 收集并过滤覆盖率数据"
# 关键: --branch-coverage 必须加在【采集】这一步。
#       只加在 genhtml 上会得到 "branches: no data found"。
lcov --capture --directory build-cov --output-file build-cov/cov.info \
     --branch-coverage \
     --rc geninfo_unexecuted_blocks=1 \
     --ignore-errors "$IGNORE_LCOV" --quiet

# 只保留我们自己的代码: 去掉系统头文件、下载的 googletest、测试文件本身
lcov --remove build-cov/cov.info \
     '/usr/*' '*/_deps/*' '*/gtest*' '*/test_*' \
     --output-file build-cov/cov.info \
     --branch-coverage \
     --ignore-errors "$IGNORE_LCOV" --quiet

echo "==> 4/4 生成 HTML (重点看 Branch 那一列)"
genhtml build-cov/cov.info \
        --output-directory build-cov/html \
        --branch-coverage \
        --ignore-errors "$IGNORE_HTML" --quiet

echo
echo "======================== 结果 ========================"
echo "用浏览器打开:  build-cov/html/index.html"
echo "提示: 页面上方可以按 Branch 列排序, 找没覆盖满的那一行"
echo
# 不要用 lcov --list: lcov 2.0 的这个子命令有 bug, 会打印出 "333% functions" 这种数字。
# 这里直接解析 cov.info, 数字和 genhtml 页面上的一致。
awk -F: -v root="$(pwd)/" '
  /^SF:/   { f = $2; sub(root, "", f); lf = lh = bf = bh = 0 }
  /^DA:/   { split($2, a, ","); lf++; if (a[2] + 0 > 0) lh++ }
  /^BRDA:/ { split($2, a, ","); bf++; if (a[4] != "-" && a[4] + 0 > 0) bh++ }
  /^end_of_record/ {
    printf "  %-26s 行 %3d/%-3d %6.1f%%   分支 %3d/%-3d %6.1f%%\n",
           f, lh, lf, (lf ? 100 * lh / lf : 100),
              bh, bf, (bf ? 100 * bh / bf : 100)
    f = ""
  }' build-cov/cov.info
