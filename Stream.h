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
#include <numeric>

template<typename T>
class VectorStream;


template<typename T>
class VectorStream2;

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

    template <typename T>
    static auto make2(std::vector<T> v) {
        return VectorStream2<T>(v);
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

    template <typename fun>
    auto launch(fun functor){ return std::async(std::launch::async, functor);}

    template<typename U, typename fun>
    auto _map(fun mapper) {
        std::vector <U> newV;
        auto v = fv.get();
        newV.resize(v.size());

        std::transform(v.begin(), v.end(), newV.begin(), mapper);
        return newV;
    }

    template <typename fun>
    auto _remove(fun predicate){
        auto v = fv.get();
        std::vector <T> newV(v);

        auto garbage = std::remove_copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage, newV.end());
        return newV;
    }

    template <typename fun>
    auto _filter(fun predicate){
        auto v = fv.get();
        std::vector <T> newV(v);

        auto garbage = std::copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage, newV.end());
        return newV;
    }

    template <typename fun>
    auto _reduce(fun reducer, T init){
        auto v = fv.get();
        return std::accumulate(v.begin(), v.end(), init, reducer);
    }

    template <typename fun>
    auto _forEach(fun action){
        auto v = fv.get();
        std::for_each(v.begin(), v.end(), action);

        return v;
    }

public:
    explicit VectorStream(promiseVector v) : fv(v.get_future()) {
    }
    explicit VectorStream(futureVector v) : fv(std::move(v)) {}

    template<typename U, typename fun>
    auto map(fun mapper) {
        auto fu = launch([=](){ return _map<U>(mapper); });
        return VectorStream<U>(std::move(fu));
    }

    template<typename fun>
    auto map(fun mapper) {
        auto fu = launch([=](){ return _map<T>(mapper); });
        return VectorStream<T>(std::move(fu));
    }

    template <typename fun>
    auto remove(fun predicate){
        auto fu = launch([&](){ return this->_remove(predicate); });
        return VectorStream<T>(std::move(fu));
    }

    template <typename fun>
    auto filter(fun predicate){
        auto fu = launch([=](){ return _filter(predicate); });
        return VectorStream<T>(std::move(fu));
    }

    template <typename fun>
    auto reduce(fun reducer, T init){
        auto fu = launch([=](){ return _reduce(reducer, init); });
        return fu;
    }

    template <typename fun>
    auto forEach(fun action){
        auto fu = launch([=](){ return _forEach(action); });
        return VectorStream<T>(std::move(fu));
    }

    auto get() { return fv.get(); }

    template <typename fun>
    auto collect(fun collector){
        return fv;
    }

    auto operator|(const VectorStream &rhs) const {
        return true;
    }
};

///@deprecated: for performance testing purposes only.
template<typename T>
class [[deprecated("use \'VectorStream\' instead.")]] VectorStream2 : public Stream {
private:
    std::vector<T> v;

public:
    explicit VectorStream2(const std::vector<T> &v) : v(v) {}

    template<typename U, typename fun>
    auto flatmap(fun mapper) {
        std::vector <U> newV;
        newV.resize(v.size());

        std::transform(v.begin(), v.end(), newV.begin(), mapper);
        return VectorStream2<U>(newV);
    }

    template <typename fun>
    auto remove(fun predicate){
        std::vector <T> newV;
        newV.resize(v.size());

        auto garbage = std::remove_copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage, newV.end());

        return VectorStream2<T>(newV);
    }

    template <typename fun>
    auto filter(fun predicate){
        std::vector <T> newV;
        newV.resize(v.size());

        auto garbage = std::copy_if(v.begin(), v.end(), newV.begin(), predicate);
        newV.erase(garbage, newV.end());

        return VectorStream2<T>(newV);
    }


    template <typename fun>
    auto reduce(fun reducer, T init){
        return std::accumulate(v.begin(), v.end(), init);
    }

    template <typename fun>
    auto &forEach(fun action){
        std::for_each(v.begin(), v.end(), action);

        return *this;
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
