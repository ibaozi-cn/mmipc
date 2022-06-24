//
// Created by didi on 2022/6/23.
//

#include <errno.h>
#include "MMIPC.h"
#include "AndroidLog.h"

using namespace std;

void MMIPC::initMMAP() {
    if (!isFileValid()) {
        //fd open failed
        ALOGD("fd open failed");
        return;
    }
    ALOGD("start mmap");
    shared_mem = (char *) ::mmap(shared_mem, MAX_INPUT_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED,
                                 fd, 0);
    if (shared_mem == MAP_FAILED) {
        ALOGD("mmap failed");
        shared_mem = nullptr;
    }
    ALOGD("mmap success");
}

bool MMIPC::open(const std::string &path) {
    if (isFileValid()) {
        return true;
    }
    fd = ::open(path.c_str(), O_RDWR | O_CREAT, S_IRWXU);
    if (!isFileValid()) {
        //fail to open
        ALOGD("fail to open [%s], %d(%s)", path.c_str(), errno, strerror(errno));
        return false;
    }
    //
    ALOGD("open fd[%d], %s", fd, path.c_str());
    return true;
}

void MMIPC::close() {
    if (isFileValid()) {
        if (::close(fd) == 0) {
            fd = -1;
        } else {
            //fail to close
        }
    }
}
