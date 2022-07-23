//
// Created by 张占永 on 2022/7/23.
//

#ifndef MMIPC_SINGLETION_H
#define MMIPC_SINGLETION_H

template<typename T>
class Singletion {
public:
    static T &get_instance() {
        // C++11保证静态变量的线程安全
        static T t;
        return t;
    }

    Singletion() = delete;

    Singletion(const Singletion &) = delete;

    Singletion &operator=(const Singletion &) = delete;
};

#endif //MMIPC_SINGLETION_H
