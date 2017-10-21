//
// Created by Aria Pahlavan on 10/20/17.
//

#ifndef CPPSTREAM_STREAM_H
#define CPPSTREAM_STREAM_H

#include <vector>
#include "VectorStream.h"

using namespace std;

class Stream {
private:

public:
    template <typename T>
    static Stream make(vector<T> v) {
        return VectorStream();
    }

    template <typename fun>
    virtual Stream map(fun mapper) = 0;

    template <typename fun>
    virtual Stream filter(fun predicate) = 0;

    virtual Stream reduce() = 0;

    template <typename fun>
    virtual Stream collect(fun collector) = 0;
};

#endif //CPPSTREAM_STREAM_H
