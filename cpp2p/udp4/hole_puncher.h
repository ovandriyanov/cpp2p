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

#include "cpp2p/event_loop.h"
#include "cpp2p/util.h"
#include "cpp2p/udp4/error.h"
#include "cpp2p/udp4/peer_handle.h"

namespace cpp2p {
namespace udp4 {

class hole_puncher_t
{
public:
    struct endpoint_t
    {
        uint32_t address;
        uint16_t port;
    };
    using hole_t = two_t<endpoint_t>;
    using cb_t = std::function<void(std::exception_ptr, hole_t&&)>;
    struct context_t
    {
        event_loop_t* event_loop;
    };

    hole_puncher_t(const context_t& context);
    void start(const cb_t& callback, const two_t<peer_handle_t*>& peers);
    void stop();

private:
    using one_cb_t = std::function<void(peer_handle_t::error_code_t, uint16_t port)>;
    class peer_t;
    class result_t;
    void bind(peer_t& ph);
    void send_cookie(peer_t& sender, peer_t& receiver);
    void recv_cookie(peer_t& receiver, peer_t& sender);
    void send_response_cookie(peer_t& sender, peer_t& receiver);

    void do_bind(peer_handle_t& ph, uint16_t port, const std::function<void()>& cb);
    void do_recv(peer_t& receiver, peer_t& sender,
                 const std::function<void()>& cb);
    void do_send(peer_t& sender, peer_t& receiver,
                 std::string payload, const std::function<void()>& cb);


    void finish(std::exception&& ex);
    void finish(hole_t&&);
    static void do_finish(const std::shared_ptr<result_t>& result_);
    void stop_peers();

    struct result_t {
        cb_t cb;
        std::exception_ptr ex;
        hole_t hole;
    };
    context_t context_;
    std::shared_ptr<result_t> result_;

    struct peer_t
    {
        peer_handle_t* handle;
        uint16_t port;
        std::string cookie_out;
    };
    two_t<peer_t> peers_;
    unsigned done_count_;
    bool response_cookie_sent_;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_HOLE_PUNCHER_H
