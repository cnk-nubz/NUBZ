#ifndef UTILS__FP_ALGORITHM__H
#define UTILS__FP_ALGORITHM__H

#include <algorithm>
#include <functional>

using namespace std::placeholders;

namespace utils {

template <class InCont, class OutCont, class F>
void transform(const InCont &inCont, OutCont &outCont, F &&f) {
    std::transform(inCont.begin(), inCont.end(), std::back_inserter(outCont), std::forward<F>(f));
}

template <class Container, class F>
bool all_of(const Container &cont, F &&f) {
    return std::all_of(cont.begin(), cont.end(), std::forward<F>(f));
}

template <class Container, class F>
bool any_of(const Container &cont, F &&f) {
    return std::any_of(cont.begin(), cont.end(), std::forward<F>(f));
}

template <class Container, class F>
bool none_of(const Container &cont, F &&f) {
    return std::none_of(cont.begin(), cont.end(), std::forward<F>(f));
}

template <class Container, class T>
std::size_t count(const Container &cont, const T &t) {
    return std::count(cont.begin(), cont.end(), t);
}

template <class A, class B, class F>
void for_each2(const A &as, const B &bs, F &&f) {
    auto aIt = as.begin();
    auto aEnd = as.end();
    auto bIt = bs.begin();
    while (aIt != aEnd) {
        f(*aIt, *bIt);
        ++aIt;
        ++bIt;
    }
};
}

#endif