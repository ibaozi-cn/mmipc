//
// Created by 张占永 on 2022/7/21.
//

#ifndef MMIPC_MUTEX_H
#define MMIPC_MUTEX_H

#include "AndroidLog.h"

class Mutex {
public:
    Mutex();

    ~Mutex();

    // lock or unlock the mutex
    void lock();

    void unlock();

    // lock if possible; returns 0 on success, error otherwise
    void tryLock();

    // Manages the mutex automatically. It'll be locked when Autolock is
    // constructed and released when Autolock goes out of scope.
    class Autolock {
    public:
        inline Autolock(Mutex &mutex) : mLock(mutex) { mLock.lock(); }

        inline Autolock(Mutex *mutex) : mLock(*mutex) { mLock.lock(); }

        inline ~Autolock() { mLock.unlock(); }

    private:
        Mutex &mLock;
    };

private:

    Mutex(const Mutex &);

    Mutex &operator=(const Mutex &);

    struct shm_mutex *pic_mutex;
};

struct shm_mutex {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};

inline Mutex::Mutex() {
    int fd = open("/data/user/0/com.zzy.mmipc/files/test.ac", O_CREAT | O_RDWR, 0777);
    ALOGE("SHARED %d",fd);
    ftruncate(fd, sizeof(*pic_mutex));
    pic_mutex = static_cast<shm_mutex *>(mmap(NULL, sizeof(struct shm_mutex),
                                              PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    close(fd);
    memset(pic_mutex,0,sizeof(*pic_mutex));
    pthread_mutexattr_init(&pic_mutex->mutexattr);
    pthread_mutexattr_setpshared(&pic_mutex->mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&pic_mutex->mutex, &pic_mutex->mutexattr);
    pthread_mutexattr_destroy(&pic_mutex->mutexattr);
}

inline Mutex::~Mutex() {
    pthread_mutex_destroy(&pic_mutex->mutex);
}

inline void Mutex::lock() {
    pthread_mutex_lock(&pic_mutex->mutex);
}

inline void Mutex::unlock() {
    pthread_mutex_unlock(&pic_mutex->mutex);
}

inline void Mutex::tryLock() {
    pthread_mutex_trylock(&pic_mutex->mutex);
}

typedef Mutex::Autolock AutoMutex;

#endif //MMIPC_MUTEX_H
