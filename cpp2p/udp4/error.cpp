/*
 *  error.cpp
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 03/05/2016.
 *
 */

#include "cpp2p/udp4/error.h"

namespace cpp2p {
namespace udp4 {

hole_punching_error_t::hole_punching_error_t(const std::string& what)
: std::runtime_error(what)
{
}

algorithm_failure_t::algorithm_failure_t(code_t code, const std::string& what)
: hole_punching_error_t(what)
, code_(code)
{
}

algorithm_failure_t::code_t algorithm_failure_t::code() const
{
    return code_;
}

fatal_peer_error_t::fatal_peer_error_t(peer_t which_peer,
                                       std::exception_ptr nested_error,
                                       const std::string& what)
: hole_punching_error_t(what)
, which_peer_(which_peer)
, nested_error_(nested_error)
{
}

std::exception_ptr fatal_peer_error_t::nested_error() const
{
    return nested_error_;
}

fatal_peer_error_t::peer_t fatal_peer_error_t::which_peer() const
{
    return which_peer_;
}

} // namespace udp4
} // namespace cpp2p
