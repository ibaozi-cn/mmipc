//
// Created by didi on 2022/6/23.
//

#include <cerrno>
#include "MMIPC.h"
#include "AndroidLog.h"
#include "Configs.h"

using namespace std;


void MMIPC::reloadMmap(const string &dir) {
    pid_t pid = getpid();
    ALOGE("pid[%d]", pid);
    m_path = configs::get_instance().getDefaultIpcFilePath();
    if (isFileValid()) {
        doCleanMemoryCache(false);
    }
    if (!open()) {
        ALOGD("fail to open [%s], %d(%s)", m_path.c_str(), errno, strerror(errno));
    } else {
        m_file_lock = new FileLock(m_fd);
        AutoFileLock autoFileLock(m_file_lock, ExclusiveLockType);
        configs::get_instance().getFileSize(m_fd, m_file_size);
        ALOGD("getFileSize m_file_size %zu, default_mmap_size %zu", m_file_size, default_mmap_size);
        if (m_file_size != default_mmap_size) {
            truncate(default_mmap_size);
        } else {
            MMIPC::mmap();
        }
    }
}

bool MMIPC::open() {
    if (isFileValid()) {
        return true;
    }
    m_fd = ::open(m_path.c_str(), O_RDWR | O_CREAT, S_IRWXU);
    if (!isFileValid()) {
        //fail to open
        return false;
    }
    //
    ALOGD("open m_fd[%d], %s", m_fd, m_path.c_str());
    return true;
}

void MMIPC::close() {
    if (isFileValid()) {
        if (::close(m_fd) == 0) {
            m_fd = -1;
        } else {
            //fail to close
        }
    }
}

void MMIPC::setData(const string &key, const string &value) {
//    AutoMutex autoMutex(m_mutex_lock);
    pthread_mutex_lock(&mutex);
    AutoFileLock autoFileLock(m_file_lock, ExclusiveLockType);
    string content = key + ":" + value + ",";
//    ALOGD("setData content=%s", content.c_str());
    size_t numberOfBytes = content.length();
    if (m_position + numberOfBytes > m_file_size) {
        auto msg = "m_position: " + to_string(m_position) + ", numberOfBytes: " +
                   to_string(numberOfBytes) +
                   ", m_file_size: " + to_string(m_file_size);
        throw out_of_range(msg);
    }
    m_position = strlen(m_ptr);
    memcpy(m_ptr + m_position, (void *) content.c_str(), numberOfBytes);
//    ALOGD("setData success m_ptr.len=%d", m_position + numberOfBytes);
    pthread_mutex_unlock(&mutex);
}

string MMIPC::getData(const string &key, const string &value) {
    return m_ptr;
}

bool MMIPC::truncate(size_t size) {
    if (!isFileValid()) {
        return false;
    }
    auto oldSize = m_file_size;
    m_file_size = size;
    if (::ftruncate(m_fd, static_cast<off_t>(m_file_size)) != 0) {
        ALOGE("truncate failed [%s] to size %zu, %s", m_path.c_str(), m_file_size, strerror(errno));
        m_file_size = oldSize;
        return false;
    }
    return MMIPC::mmap();
}

bool MMIPC::mmap() {
    m_ptr = (char *) ::mmap(m_ptr, default_mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
    if (m_ptr == MAP_FAILED) {
        doCleanMemoryCache(true);
        ALOGD("mmap failed");
        return false;
    }
    ALOGD("mmap success");
    return true;
}

void MMIPC::doCleanMemoryCache(bool forceClean) {
    if (!forceClean) {
        return;
    }
    if (m_ptr && m_ptr != MAP_FAILED) {
        if (munmap(m_ptr, m_file_size) != 0) {
            ALOGE("munmap fail [%s], %s", m_path.c_str(), strerror(errno));
        }
    }
    m_ptr = nullptr;
    m_path = nullptr;
    m_file_size = 0;
    m_position = 0;
    default_mmap_size = 0;
    close();
    delete &m_mutex_lock;
    delete m_file_lock;
    pthread_mutex_destroy(&mutex);
}

