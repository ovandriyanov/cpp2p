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
    hole_punching_error_t(std::string what);
};

class algorithm_failure_t : public hole_punching_error_t
{
public:
    enum code_t
    {
        EC_TOO_MANY_RETRIES
    };

    algorithm_failure_t(code_t code);
    code_t code() const;

private:
    code_t code_;
};

class culprit_peer_t
{
public:
    culprit_peer_t(std::string name);
    const std::string& name() const;

private:
    std::string name_;
};

class peer_error_t : public hole_punching_error_t, public culprit_peer_t
{
public:
    enum code_t
    {
        EC_PROTOCOL_VIOLATION,
        EC_TOO_MANY_ERRORS
    };

    peer_error_t(std::string peer_name, code_t code);
    code_t code() const;

private:
    code_t code_;
};

class fatal_peer_error_t : public hole_punching_error_t, public culprit_peer_t
{
public:
    fatal_peer_error_t(std::string peer_name, std::exception_ptr nested_error, std::string what);
    std::exception_ptr nested_error() const;

private:
    std::exception_ptr nested_error_;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_ERROR_H
