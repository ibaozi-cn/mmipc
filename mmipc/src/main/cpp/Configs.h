//
// Created by 张占永 on 2022/7/23.
//

#ifndef MMIPC_CONFIGS_H
#define MMIPC_CONFIGS_H

#include "Singletion.h"

using namespace std;

constexpr auto FILE_SEPARATOR = "/";
constexpr auto DEFAUL_IPC_FILE = "default_mmap.ipc";
constexpr auto DEFAUL_MUTEX_FILE = "default_mutex.ipc";

class Configs {

public:
     string g_rootDir;

     string getDefaultMutexFilePath() {
        return g_rootDir + FILE_SEPARATOR + DEFAUL_MUTEX_FILE;
    }

     bool getFileSize(int fd, size_t &size) {
        struct stat st = {};
        if (fstat(fd, &st) != -1) {
            size = (size_t) st.st_size;
            ALOGD("m_fd size[%d]", size);
            return true;
        }
        return false;
    }

     size_t getPageSize() {
        return static_cast<size_t>(getpagesize());
    }

     string getDefaultIpcFilePath() {
        return g_rootDir + FILE_SEPARATOR + DEFAUL_IPC_FILE;
    }
};

typedef Singletion<Configs> configs;

#endif //MMIPC_CONFIGS_H
