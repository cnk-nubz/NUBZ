#ifndef UTILS__TYPE_TRAITS__H
#define UTILS__TYPE_TRAITS__H

#include <type_traits>

namespace utils {
namespace types {

// ::value = true if type A is on the List
template <class A, class... List>
struct find_type : std::false_type {};

template <class A, class... List>
struct find_type<A, A, List...> : std::true_type {};

template <class A, class Head, class... Tail>
struct find_type<A, Head, Tail...> : find_type<A, Tail...> {};

// ::value = true if B::value == true for all given Bs
template <class... B>
struct all_of : std::true_type {};

template <class Head, class... Tail>
struct all_of<Head, Tail...> {
    static constexpr bool value = Head::value && all_of<Tail...>::value;
};
}
}

#endif