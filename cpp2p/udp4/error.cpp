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

hole_punching_error_t::hole_punching_error_t(std::string what)
: std::runtime_error(std::move(what))
{
}

static std::string str(algorithm_failure_t::code_t code)
{
    switch(code) {
        case algorithm_failure_t::EC_TOO_MANY_RETRIES: return "Too many retries";
    }
    return "Unexpected error";
}

algorithm_failure_t::algorithm_failure_t(code_t code)
: hole_punching_error_t("Unable to punch NAT hole: " + str(code))
, code_(code)
{
}

algorithm_failure_t::code_t algorithm_failure_t::code() const
{
    return code_;
}

culprit_peer_t::culprit_peer_t(std::string name)
: name_(std::move(name))
{
}

const std::string& culprit_peer_t::name() const
{
    return name_;
}

static std::string str(peer_error_t::code_t code)
{
    switch(code) {
        case peer_error_t::EC_PROTOCOL_VIOLATION: return "Protocol violation";
        case peer_error_t::EC_TOO_MANY_ERRORS: return "Too many errors";
    }
    return "Unexpected error";
}

peer_error_t::peer_error_t(std::string peer_name, code_t code)
: hole_punching_error_t("Peer \"" + peer_name + "\": " + str(code))
, culprit_peer_t(std::move(peer_name))
, code_(code)
{
}

peer_error_t::code_t peer_error_t::code() const
{
    return code_;
}

fatal_peer_error_t::fatal_peer_error_t(std::string which_peer, std::exception_ptr nested_error, std::string what)
: hole_punching_error_t(std::move(what))
, culprit_peer_t(std::move(which_peer))
, nested_error_(nested_error)
{
}

std::exception_ptr fatal_peer_error_t::nested_error() const
{
    return nested_error_;
}

} // namespace udp4
} // namespace cpp2p
