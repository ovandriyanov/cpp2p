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
#include <utility>

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

template <typename T> using two_t = std::pair<T, T>;
using hole_t = two_t<endpoint_t>;

class hole_puncher_t
{
public:
    class delegate_t
    {
    public:
        virtual ~delegate_t() = default;
        virtual timer_t& timer() = 0;
        virtual event_loop_t& event_loop() = 0;
        virtual two_t<peer_handle_t&> peer_handles() = 0;
        virtual void hole_punched(hole_t&&) = 0;
        virtual void error_occurred(hole_punching_error_t&&) = 0;
    };

    virtual ~hole_puncher_t() = default;
    virtual void start(std::shared_ptr<delegate_t>) = 0;
    virtual void stop() = 0;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_HOLE_PUNCHER_H
