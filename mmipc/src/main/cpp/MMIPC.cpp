//
// Created by didi on 2022/6/23.
//

#include <cerrno>
#include "MMIPC.h"
#include "AndroidLog.h"

using namespace std;

extern bool getFileSize(int fd, size_t &size);

extern size_t getPageSize();

extern bool zeroFillFile(int fd, size_t startPos, size_t size);

void MMIPC::reloadMmap(const string &dir) {
    m_path = getDefaultIpcFilePath(dir);
    if (isFileValid()) {
        doCleanMemoryCache(false);
    }
    default_mmap_size = getPageSize();
    if (!open()) {
        ALOGD("fail to open [%s], %d(%s)", m_path.c_str(), errno, strerror(errno));
    } else {
        getFileSize(m_fd, m_size);
        if (m_size < default_mmap_size || (m_size % default_mmap_size != 0)) {
            size_t roundSize = (m_size / default_mmap_size + 1) * default_mmap_size;
            truncate(roundSize);
        } else {
            auto ret = mmap();
            if (!ret) {
                doCleanMemoryCache(true);
            }
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
    string content = key + ":" + value;
    if (m_position != 0) {
        content = "," + content;
    }
    ALOGD("setData content=%s", content.c_str());
    size_t numberOfBytes = content.length();
    if (m_position + numberOfBytes > m_size) {
        auto msg = "m_position: " + to_string(m_position) + ", numberOfBytes: " +
                   to_string(numberOfBytes) +
                   ", m_size: " + to_string(m_size);
        throw out_of_range(msg);
    }
    memcpy(m_ptr + m_position, (void *) content.c_str(), numberOfBytes);
    m_position += numberOfBytes;
    ALOGD("setData success m_ptr=%s", m_ptr);
}

string MMIPC::getData(const string &key, const string &value) {
    return m_ptr;
}

bool MMIPC::truncate(size_t size) {
    if (!isFileValid()) {
        return false;
    }
    if (size == m_size) {
        return true;
    }
    auto oldSize = m_size;
    m_size = size;
    if (::ftruncate(m_fd, static_cast<off_t>(m_size)) != 0) {
        ALOGE("truncate failed [%s] to size %zu, %s", m_path.c_str(), m_size, strerror(errno));
        m_size = oldSize;
        return false;
    }
    if (oldSize > m_size) {
        if (!zeroFillFile(m_fd, oldSize, m_size - oldSize)) {
            ALOGE("zeroFile fail [%s] to size %zu, %s", m_path.c_str(), m_size,
                  strerror(errno));
            m_size = oldSize;
            return false;
        }
    }
    if (m_ptr) {
        if (munmap(m_ptr, oldSize) != 0) {
            ALOGE("munmap fail [%s], %s", m_path.c_str(), strerror(errno));
        }
    }
    auto ret = mmap();
    if (!ret) {
        doCleanMemoryCache(true);
    }
    return ret;
}

bool MMIPC::mmap() {
    m_ptr = (char *) ::mmap(m_ptr, m_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, m_fd, 0);
    if (m_ptr == MAP_FAILED) {
        ALOGD("mmap failed");
        m_ptr = nullptr;
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
        if (munmap(m_ptr, m_size) != 0) {
            ALOGE("munmap fail [%s], %s", m_path.c_str(), strerror(errno));
        }
    }
    m_ptr = nullptr;
    close();
    m_size = 0;
}

string MMIPC::getDefaultIpcFilePath(const string &dir) {
    return dir + FILE_SEPARATOR + DEFAUL_IPC_FILE;
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


bool zeroFillFile(int fd, size_t startPos, size_t size) {
    if (fd < 0) {
        return false;
    }

    if (lseek(fd, static_cast<off_t>(startPos), SEEK_SET) < 0) {
        ALOGE("lseek fail m_fd[%d], error:%s", fd, strerror(errno));
        return false;
    }

    static const char zeros[4096] = {};
    while (size >= sizeof(zeros)) {
        if (write(fd, zeros, sizeof(zeros)) < 0) {
            ALOGE("write fail m_fd[%d], error:%s", fd, strerror(errno));
            return false;
        }
        size -= sizeof(zeros);
    }
    if (size > 0) {
        if (write(fd, zeros, size) < 0) {
            ALOGE("write fail m_fd[%d], error:%s", fd, strerror(errno));
            return false;
        }
    }
    return true;
}