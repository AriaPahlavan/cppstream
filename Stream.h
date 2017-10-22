//
// Created by Aria Pahlavan on 10/20/17.
//

#ifndef CPPSTREAM_STREAM_H
#define CPPSTREAM_STREAM_H

#include "VectorStream.h"

using namespace std;

class Stream {
private:

public:
    template <typename T>
    static auto make(vector<T> v) {
        return VectorStream<T>(v);
    }
};

#endif //CPPSTREAM_STREAM_H
