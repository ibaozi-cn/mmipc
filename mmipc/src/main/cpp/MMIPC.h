//
// Created by didi on 2022/6/23.
//
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

using namespace std;

#ifndef MMIPC_MMIPC_H
#define MMIPC_MMIPC_H

static int fd = -1;
static void *shared_mem;

class MMIPC {
    static const int MAX_INPUT_LENGTH = 1024;
public:
    static bool open(const string &path);
    static void close();
    static void initMMAP();
    static bool isFileValid()  { return fd >= 0; }
};


#endif //MMIPC_MMIPC_H
