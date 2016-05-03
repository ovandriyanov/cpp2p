/*
 *  error.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 03/05/2016.
 *
 */

#ifndef CPP2P_UDP4_ERROR_H
#define CPP2P_UDP4_ERROR_H

#include <stdexcept>

namespace cpp2p {
namespace udp4 {

class hole_punching_error_t : public std::runtime_error
{
public:
    hole_punching_error_t(const std::string& what);
};

class algorithm_failure_t : public hole_punching_error_t
{
public:
    enum class code_t
    {
        TOO_MANY_RETRIES,
        FIRST_PEER_TOO_MANY_ERRORS,
        SECOND_PEER_TOO_MANY_ERRORS
    };

    algorithm_failure_t(code_t code, const std::string& what);
    code_t code() const;

private:
    code_t code_;
};

class fatal_peer_error_t : public hole_punching_error_t
{
public:
    enum class peer_t { FIRST, SECOND };

    fatal_peer_error_t(peer_t which_peer, std::exception_ptr nested_error,
                       const std::string& what);
    peer_t which_peer() const;
    std::exception_ptr nested_error() const;

private:
    peer_t which_peer_;
    std::exception_ptr nested_error_;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_ERROR_H
