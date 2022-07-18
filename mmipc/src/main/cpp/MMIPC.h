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

constexpr auto FILE_SEPARATOR = "/";
constexpr auto DEFAUL_IPC_FILE = "default_mmap.ipc";

class MMIPC {
    int m_fd = -1;
    string m_path;
    char *m_ptr;
    size_t m_file_size;
    size_t default_mmap_size;
    size_t m_position = 0;

public:
    ~MMIPC() { doCleanMemoryCache(true); }

    void doCleanMemoryCache(bool forceClean);

    bool open();

    void close();

    bool truncate(size_t size);

    bool mmap();

    void reloadMmap(const string &dir);

    void setData(const string &key, const string &value);

    string getData(const string &key, const string &value);

    bool isFileValid() { return m_fd >= 0; }

    static string getDefaultIpcFilePath(const string &dir);
};


#endif //MMIPC_MMIPC_H
