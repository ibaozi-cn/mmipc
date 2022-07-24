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
#include "FileLock.h"

using namespace std;

#ifndef MMIPC_MMIPC_H
#define MMIPC_MMIPC_H

class MMIPC {
    int m_fd;
    string m_path;
    char *m_ptr;
    size_t m_file_size;
    size_t default_mmap_size;
    size_t m_position;
    ShmMutex m_mutex_lock;
    FileLock *m_file_lock;
    pthread_mutex_t mutex;

public:

    // 一般一个缓存页4k，乘以1024，4M
    MMIPC() : m_fd(-1), m_ptr(nullptr), m_file_size(0),
              default_mmap_size(configs::get_instance().getPageSize() * 1024),m_position(0){
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
        pthread_mutex_init(&mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }

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
