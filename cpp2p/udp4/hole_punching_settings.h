/*
 *  cpp2p/udp4/hole_punching_settings.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 15/05/2016.
 *
 */

#ifndef CPP2P_UDP4_HOLE_PUNCHING_SETTINGS_H
#define CPP2P_UDP4_HOLE_PUNCHING_SETTINGS_H

#include <stdint.h>

#include "cpp2p/util.h"

namespace cpp2p {
namespace udp4 {

class hole_punching_settings_t
{
public:
    hole_punching_settings_t();

    static const uint16_t DESIRED_PORT_ANY = 0;
    hole_punching_settings_t& desired_ports(const two_t<uint16_t>& ports);
    const two_t<uint16_t>& desired_ports() const;

private:
    two_t<uint16_t> desired_ports_;
};

inline hole_punching_settings_t::hole_punching_settings_t()
: desired_ports_(DESIRED_PORT_ANY, DESIRED_PORT_ANY)
{
}

inline hole_punching_settings_t&
hole_punching_settings_t::desired_ports(const two_t<uint16_t>& ports)
{
    desired_ports_ = ports;
    return *this;
}

inline const two_t<uint16_t>& hole_punching_settings_t::desired_ports() const
{
    return desired_ports_;
}

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_HOLE_PUNCHING_SETTINGS_H
