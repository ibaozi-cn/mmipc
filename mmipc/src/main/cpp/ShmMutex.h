//
// Created by 张占永 on 2022/7/21.
//

#ifndef MMIPC_SHMMUTEX_H
#define MMIPC_SHMMUTEX_H

#include "AndroidLog.h"
#include "Configs.h"

typedef int status_t;

using namespace std;

class ShmMutex {
public:
    enum {
        PRIVATE = 0,
        SHARED = 1
    };

    ShmMutex();

    ~ShmMutex();

    // lock or unlock the mutex
    status_t lock();

    void unlock();

    // lock if possible; returns 0 on success, error otherwise
    status_t tryLock();

    /**
     * 该方法，创建共享内存的Mutex，实现跨进程互斥锁
     * @param dir
     */
    void createShmMutex(string dir);

    // Manages the mutex automatically. It'll be locked when Autolock is
    // constructed and released when Autolock goes out of scope.
    class Autolock {
    public:
        inline Autolock(ShmMutex &mutex) : mLock(mutex) { mLock.lock(); }

        inline Autolock(ShmMutex *mutex) : mLock(*mutex) { mLock.lock(); }

        inline ~Autolock() { mLock.unlock(); }

    private:
        ShmMutex &mLock;
    };

private:

    ShmMutex(const ShmMutex &);

    ShmMutex &operator=(const ShmMutex &);

    struct shm_mutex *shmMutex;

};

struct shm_mutex {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};

inline ShmMutex::ShmMutex() {
    createShmMutex(configs::get_instance().getDefaultMutexFilePath());
}

inline ShmMutex::~ShmMutex() {
    pthread_mutex_destroy(&shmMutex->mutex);
}

inline status_t ShmMutex::lock() {
    return pthread_mutex_lock(&shmMutex->mutex);
}

inline void ShmMutex::unlock() {
    pthread_mutex_unlock(&shmMutex->mutex);
}

inline status_t ShmMutex::tryLock() {
    return pthread_mutex_trylock(&shmMutex->mutex);
}

inline void ShmMutex::createShmMutex(string dir) {
    int fd = open(dir.c_str(), O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        ALOGD("open fail for mutex: fd[%d], %s", fd, dir.c_str());
        return;
    }
    ALOGD("open success for mutex: fd[%d], %s", fd, dir.c_str());
    ftruncate(fd, sizeof(struct shm_mutex));
    shmMutex = static_cast<shm_mutex *>(mmap(NULL, sizeof(struct shm_mutex),
                                             PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (shmMutex == MAP_FAILED) {
        ALOGD("mutex mmap failed");
        return;
    }
    if (close(fd) == 0) {
        fd = -1;
    } else {
        //fail to close
        ALOGD("mutex fd close failed");
    }
    memset(shmMutex, 0, sizeof(struct shm_mutex));
    pthread_mutexattr_init(&shmMutex->mutexattr);
    pthread_mutexattr_setpshared(&shmMutex->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shmMutex->mutex, &shmMutex->mutexattr);
    pthread_mutexattr_destroy(&shmMutex->mutexattr);
}


typedef ShmMutex::Autolock AutoMutex;

#endif //MMIPC_SHMMUTEX_H
