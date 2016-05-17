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
        SAME_PEER,
        ALREADY_IN_PROGRESS,
        TOO_MANY_RETRIES
    };

    algorithm_failure_t(code_t code, const std::string& what);
    code_t code() const;

private:
    code_t code_;
};

class culprit_peer_t
{
public:
    enum class peer_t { FIRST, SECOND };
    culprit_peer_t(peer_t which);
    peer_t which_peer();

private:
    peer_t which_peer_;
};

class peer_error_t : public hole_punching_error_t, public culprit_peer_t
{
public:
    enum class code_t
    {
        PROTOCOL_VIOLATION,
        TOO_MANY_ERRORS
    };

    peer_error_t(peer_t which, code_t code, const std::string& what);
    code_t code() const;

private:
    code_t code_;
};

class fatal_peer_error_t : public hole_punching_error_t, public culprit_peer_t
{
public:
    enum class peer_t { FIRST, SECOND };

    fatal_peer_error_t(peer_t which_peer, std::exception_ptr nested_error,
                       const std::string& what);
    std::exception_ptr nested_error() const;

private:
    std::exception_ptr nested_error_;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_ERROR_H
