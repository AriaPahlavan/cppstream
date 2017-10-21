//
// Created by Aria Pahlavan on 10/20/17.
//

#ifndef CPPSTREAM_VECTORSTREAM_H
#define CPPSTREAM_VECTORSTREAM_H

#include "Stream.h"

template <typename T>
class VectorStream : public Stream{
public:

    template <typename fun>
    Stream map(fun mapper) override;

    template <typename fun>
    Stream filter(fun predicate) override;

    Stream reduce() override;

    template <typename fun>
    Stream collect(fun collector) override;
};


#endif //CPPSTREAM_VECTORSTREAM_H
