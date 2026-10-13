#include "fileio.h"

#include <fcntl.h>      // open, O_WRONLY ...
#include <unistd.h>     // read, write, close
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>

// ================= 方式一: C++ 流 =================

long write_text_stream(const std::string& path, const std::string& text) {
    std::ofstream ofs(path, std::ios::binary | std::ios::trunc);
    if (!ofs) return -1;
    ofs.write(text.data(), static_cast<std::streamsize>(text.size()));
    if (!ofs) return -1;
    return static_cast<long>(text.size());   // 析构时自动 flush + close
}

std::string read_text_stream(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return {};
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

// ================= 方式二: 系统调用 =================

long write_text_syscall(const std::string& path, const std::string& text) {
    // O_CREAT: 不存在就创建    O_TRUNC: 存在就清空    0644: 权限 rw-r--r--
    int fd = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    // write 可能只写一部分, 必须循环写到写完 —— 这就是系统调用的"麻烦之处"
    const char* buf  = text.data();
    std::size_t left = text.size();
    while (left > 0) {
        ssize_t n = ::write(fd, buf, left);
        if (n < 0) {
            if (errno == EINTR) continue;    // 被信号打断, 重试
            ::close(fd);
            return -1;
        }
        buf  += n;
        left -= static_cast<std::size_t>(n);
    }
    ::close(fd);
    return static_cast<long>(text.size());
}

std::string read_text_syscall(const std::string& path) {
    int fd = ::open(path.c_str(), O_RDONLY);
    if (fd < 0) return {};

    std::string out;
    char        buf[4096];
    for (;;) {
        ssize_t n = ::read(fd, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EINTR) continue;
            ::close(fd);
            return {};
        }
        if (n == 0) break;                   // 读到文件尾
        out.append(buf, static_cast<std::size_t>(n));
    }
    ::close(fd);
    return out;
}
