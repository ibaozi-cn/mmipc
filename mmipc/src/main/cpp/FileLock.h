//
// Created by 张占永 on 2022/7/24.
//

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <cerrno>
#include "AndroidLog.h"

#ifndef MMIPC_FILELOCK_H
#define MMIPC_FILELOCK_H


enum LockType {
    SharedLockType,
    ExclusiveLockType,
};

class FileLock {
    int m_fd;

    bool isFileLockValid() { return m_fd >= 0; }

public:
    explicit FileLock(int fd);

    bool lock(LockType lockType);

    bool try_lock(LockType lockType);

    bool unlock();

    // just forbid it for possibly misuse
    explicit FileLock(const FileLock &other) = delete;

    FileLock &operator=(const FileLock &other) = delete;

    class Autolock {
    public:
        inline Autolock(FileLock &flock, LockType lockType) : mFLock(flock), m_lockType(lockType) {
            mFLock.lock(m_lockType);
        }

        inline Autolock(FileLock *flock, LockType lockType) : mFLock(*flock), m_lockType(lockType) {
            mFLock.lock(m_lockType);
        }

        inline ~Autolock() { mFLock.unlock(); }

    private:
        FileLock &mFLock;
        LockType m_lockType;
    };

};

inline FileLock::FileLock(int fd)
        : m_fd(fd) {
}

inline bool FileLock::lock(LockType lockType) {
    if (!isFileLockValid()) {
        return false;
    }
    int res = flock(m_fd, lockType == SharedLockType ? LOCK_SH : LOCK_EX);
    if (res == -1) {
        ALOGD("flock lock fail: fd[%d]", m_fd);
    }
    return res == 0;
}

inline bool FileLock::try_lock(LockType lockType) {
    if (!isFileLockValid()) {
        return false;
    }
    int res = flock(m_fd, lockType == SharedLockType ? LOCK_SH | LOCK_NB : LOCK_EX | LOCK_NB);
    if (res == -1) {
        ALOGD("flock try_lock fail: fd[%d]", m_fd);
    }
    return res == 0;
}

inline bool FileLock::unlock() {
    if (!isFileLockValid()) {
        return false;
    }
    int res = flock(m_fd, LOCK_UN);
    if (res == -1) {
        ALOGD("flock unlock fail: fd[%d]", m_fd);
    }
    return res == 0;
}

typedef FileLock::Autolock AutoFileLock;

#endif //MMIPC_FILELOCK_H
