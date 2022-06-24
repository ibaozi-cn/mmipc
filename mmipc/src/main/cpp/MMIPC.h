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
    //清理缓存
    void doCleanMemoryCache(bool forceClean);
    //打开文件
    bool open();
    //关闭文件
    void close();
    //刷新文件大小
    bool truncate(size_t size);
    // mmap 映射
    bool mmap();
    // 支持重新加载mmap
    void reloadMmap(const string &path);
    // 增量更新数据
    void setData(const string &key, const string &value);
    // 获取缓存数据
    string getData(const string &key, const string &value);
    // 判断文件链接是否正常
    bool isFileValid() { return m_fd >= 0; }
};


#endif //MMIPC_MMIPC_H
