//
// Created by didi on 2022/6/23.
//
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/file.h>
#include <pthread.h>
#include "ShmMutex.h"

using namespace std;

#ifndef MMIPC_MMIPC_H
#define MMIPC_MMIPC_H

class MMIPC {
    int m_fd = -1;
    string m_path;
    char *m_ptr;
    size_t m_file_size;
    size_t default_mmap_size;
    size_t m_position = 0;
    ShmMutex mLock;

public:

    ~MMIPC() {
        doCleanMemoryCache(true);

    }

    void doCleanMemoryCache(bool forceClean);

    bool open();

    void close();

    bool truncate(size_t size);

    bool mmap();

    void reloadMmap(const string &dir);

    void setData(const string &key, const string &value);

    string getData(const string &key, const string &value);

    bool isFileValid() { return m_fd >= 0; }

};

typedef Singletion<MMIPC> mmipc;

#endif //MMIPC_MMIPC_H
