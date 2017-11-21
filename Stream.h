//
// Created by Aria Pahlavan on 10/20/17.
//

#ifndef CPPSTREAM_STREAM_H
#define CPPSTREAM_STREAM_H

#include <vector>
#include <numeric>
#include <map>
#include <future>
#include "Stream.h"

template<typename T>
class VectorStream;

template<typename M, typename N>
class MapStream;

class Stream {
public:
    template <typename T>
    static auto make(std::vector<T> v) {
        std::promise<std::vector<T>> vec;
        vec.set_value(v);
        return VectorStream<T>(move(vec));
    }

    template <typename T, typename U>
    static auto make(std::map<T, U> m) {
        return MapStream<T, U>(m);
    }
};

template<typename T>
class VectorStream : public Stream {
private:
    typedef std::promise<std::vector<T>> promiseVector;
    typedef std::future<std::vector<T>> futureVector;

    futureVector fv;

public:
    explicit VectorStream(promiseVector v) : fv(v.get_future()) {
    }
    explicit VectorStream(futureVector v) : fv(std::move(v)) {}

    template<typename U, typename fun>
    auto _flatmap(fun mapper) {
        std::vector <U> newV;
        auto v = fv.get();
        newV.resize(v.size());

        std::transform(v.begin(), v.end(), newV.begin(), mapper);
        return newV;
    }

    template<typename U, typename fun>
    auto flatmap(fun mapper) {
        auto fu = std::async(std::launch::async, [=](){ return _flatmap<U>(mapper); });
        return VectorStream<U>(std::move(fu));
    }

    template <typename fun>
    auto _remove(fun predicate){
        std::vector <T> newV;
        auto v = fv.get();
        newV.resize(v.size());

        auto garbage = std::remove_copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage);
        return newV;
    }

    template <typename fun>
    auto remove(fun predicate){
        auto fu = std::async(std::launch::async, [=](){ return _remove(predicate); });
        return VectorStream<T>(std::move(fu));
    }

    template <typename fun>
    auto _reduce(fun reducer, T init){
        auto v = fv.get();
        return std::accumulate(v.begin(), v.end(), init, reducer);
    }

    template <typename fun>
    auto reduce(fun reducer, T init){
        auto fu = std::async(std::launch::async, [=](){ return _reduce(reducer, init); });
        return fu;
    }

    template <typename fun>
    auto _filter(fun predicate){
        std::vector <T> newV;
        auto v = fv.get();
        newV.resize(v.size());

        auto garbage = std::copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage);
        return newV;
    }

    template <typename fun>
    auto filter(fun predicate){
        auto fu = std::async(std::launch::async, [=](){ return _filter(predicate); });
        return VectorStream<T>(std::move(fu));
    }

    template <typename fun>
    auto forEach(fun action){
        auto v = fv.get();
        std::for_each(v.begin(), v.end(), action);

        promiseVector pv;
        pv.set_value(v);
        return VectorStream<T>(std::move(pv));
    }

    template <typename fun>
    auto collect(fun collector){
        return fv;
    }

    auto operator|(const VectorStream &rhs) const {
        return true;
    }
};

template<typename M, typename N>
class MapStream : public Stream {
private:
    std::map<M, N> m;

public:
    explicit MapStream(const std::map<M, N> &m) : m(m) {}

    template<typename T, typename U, typename fun>
    auto flatmap(fun mapper) {
        auto newM = new std::map<T, U>(m);
    }

    template <typename fun>
    auto remove(fun predicate){
        auto newM = new std::map<int, std::string>(m);

        auto garbage = std::remove_copy_if(m.begin(), m.end(), (*newM).begin(), predicate);
        newM->erase(garbage);

        return MapStream<M, N>(*newM);
    }

    template <typename fun>
    auto filter(fun predicate){
        std::map<M, N> newM = m;

        auto garbage = std::copy_if(m.begin(), m.end(), newM.begin(), predicate);
        newM.erase(garbage);

        return MapStream<M, N>(newM);
    }

    template <typename fun>
    auto reduce(fun reducer, N init){
        return std::accumulate(m.begin(), m.end(), init, reducer);
    }

    template <typename fun>
    auto &forEach(fun action){
        std::for_each(m.begin(), m.end(), action);

        return *this;
    }

    template <typename fun>
    auto collect(fun collector){
        return m;
    }
};



#endif //CPPSTREAM_STREAM_H
