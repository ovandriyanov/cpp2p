/*
 *  hole_puncher.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 21/04/2016.
 *
 */

#ifndef CPP2P_UDP4_HOLE_PUNCHER_H
#define CPP2P_UDP4_HOLE_PUNCHER_H

#include <stdint.h>

#include <memory>

#include "cpp2p/udp4/hole_punching_settings.h"
#include "cpp2p/util.h"

namespace cpp2p {

class event_loop_t;
class timer_t;

namespace udp4 {

class hole_punching_error_t;
class peer_handle_t;

struct endpoint_t
{
    uint32_t address;
    uint16_t port;
};

using hole_t = two_t<endpoint_t>;

class hole_puncher_t
{
public:
    struct context_t
    {
        event_loop_t& event_loop;
        timer_t& timer;
        two_t<peer_handle_t&> peer_handles;
    };
    using cb_t = std::function<void(hole_punching_error_t&&, hole_t&&)>;

    virtual ~hole_puncher_t() = default;
    virtual void start(const cb_t& callback,
                       const context_t& context,
                       const hole_punching_settings_t& settings = hole_punching_settings_t()) = 0;
    virtual void stop() = 0;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_HOLE_PUNCHER_H
