/*
 *  cpp2p/util.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 15/05/2016.
 *
 */

#ifndef CPP2P_UTIL_H
#define CPP2P_UTIL_H

#include <sstream>
#include <utility>

namespace cpp2p {

template <typename T> using two_t = std::pair<T, T>;

template <typename... T>
std::string concat(const T&... args)
{
    std::ostringstream os;
    (int[]){ (os << args, 0)... };
    return os.str();
}

} // namespace cpp2p

#endif // CPP2P_UTIL_H
