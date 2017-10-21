//
// Created by Aria Pahlavan on 10/20/17.
//

#include "VectorStream.h"

template<typename fun>
Stream VectorStream::map(fun mapper) {
    return VectorStream();
}

template<typename fun>
Stream VectorStream::filter(fun predicate) {
    return VectorStream();
}

Stream VectorStream::reduce() {
    return VectorStream();
}

template<typename fun>
Stream VectorStream::collect(fun collector) {
    return VectorStream();
}