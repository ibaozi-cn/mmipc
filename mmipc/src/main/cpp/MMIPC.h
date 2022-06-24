//
// Created by didi on 2022/6/23.
//
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

using namespace std;

#ifndef MMIPC_MMIPC_H
#define MMIPC_MMIPC_H


class MMIPC {
    int m_fd = -1; // 文件句柄
    string m_path; // 磁盘文件路径，最终保存的位置
    char *m_ptr;    // mmap 内存数据
    size_t m_size;  // mmap 内存数据大小
    size_t default_mmap_size;   // 默认页缓存大小
    size_t m_position = 0;  // 当前内存数据长度所在位置

public:
    ~MMIPC() { doCleanMemoryCache(true); }
    //
    void doCleanMemoryCache(bool forceClean);

    bool open();

    void close();

    bool truncate(size_t size);

    bool mmap();

    void reloadMmap(const string &path);

    void setData(const string &key, const string &value);

    string getData(const string &key, const string &value);

    bool isFileValid() { return m_fd >= 0; }
};


#endif //MMIPC_MMIPC_H
