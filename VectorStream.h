//
// Created by Aria Pahlavan on 10/20/17.
//

#ifndef CPPSTREAM_VECTORSTREAM_H
#define CPPSTREAM_VECTORSTREAM_H

#include <vector>
#include <numeric>

using namespace std;

template<typename T>
class VectorStream {
private:
    std::vector<T> v;

public:
    explicit VectorStream(const std::vector<T> &v) : v(v) {}

    template<typename U, typename fun>
    auto flatmap(fun mapper) {
        vector <U> newV;
        newV.resize(v.size());

        transform(v.begin(), v.end(), newV.begin(), mapper);
        return VectorStream<U>(newV);
    }

    template <typename fun>
    auto remove(fun predicate){
        vector <T> newV;
        newV.resize(v.size());

        auto garbage = remove_copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage);

        return VectorStream<T>(newV);
    }

    template <typename fun>
    auto filter(fun predicate){
        vector <T> newV;
        newV.resize(v.size());

        auto garbage = copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage);

        return VectorStream<T>(newV);
    }


    template <typename fun>
    auto reduce(fun reducer, T init){
        return accumulate(v.begin(), v.end(), init);
    }

    template <typename fun>
     auto &forEach(fun action){
        for_each(v.begin(), v.end(), action);

        return *this;
    }

    template <typename fun>
    auto collect(fun collector){
        return v;
    }
};


#endif //CPPSTREAM_VECTORSTREAM_H
