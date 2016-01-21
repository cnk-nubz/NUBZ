#ifndef UTILS__FP_ALGORITHM__H
#define UTILS__FP_ALGORITHM__H

#include <algorithm>

namespace utils {

template <class InCont, class OutCont, class F>
void transform(const InCont& inCont, OutCont& outCont, F&& f) {
    std::transform(inCont.begin(), inCont.end(), std::back_inserter(outCont), f);
}

template <class Container, class F>
bool all_of(const Container& cont, F&& f) {
    return std::all_of(cont.begin(), cont.end(), f);
}

template <class Container, class F>
bool any_of(const Container& cont, F&& f) {
    return std::any_of(cont.begin(), cont.end(), f);
}

template <class Container, class F>
bool none_of(const Container& cont, F&& f) {
    return std::none_of(cont.begin(), cont.end(), f);
}
}

#endif