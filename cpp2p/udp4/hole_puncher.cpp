/*
 *  cpp2p/udp4/hole_puncher.cpp
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 12/06/2016.
 *
 */

#include "cpp2p/udp4/hole_puncher.h"

#include "cpp2p/udp4/peer_handle.h"
#include "cpp2p/udp4/error.h"

#include <assert.h>

using namespace std::placeholders;

namespace cpp2p {
namespace udp4 {

using ec = algorithm_failure_t::code_t;

static const uint16_t PORT_DEFAULT = 9876;

hole_puncher_t::hole_puncher_t(const context_t& context)
    : context_(context)
    , result_(std::make_shared<result_t>())
{
}

void hole_puncher_t::start(const cb_t& callback, const two_t<peer_handle_t*>& peers)
{
    assert(!result_->cb);
    result_->cb = callback;

    static const auto get_port = [](peer_handle_t* ph) {
        return ph->desired_port() == peer_handle_t::any_port
            ? PORT_DEFAULT
            : ph->desired_port();
    };
    peers_ = {
        { peers.first, get_port(peers.first) }
      , { peers.second, get_port(peers.second) }
    };

    if(peers.first->public_ip() == peers.second->public_ip())
        return finish(std::invalid_argument("Supplied peers are equal"));


    done_count_ = 0;
    response_cookie_sent_ = false;
    bind(peers_.first);
    bind(peers_.second);
}

void hole_puncher_t::bind(peer_t& peer)
{
    do_bind(*peer.handle, peer.port, [this, &peer]()
    {
        peer.cookie_out = concat("COOKIE_", done_count_);
        if(++done_count_ != 2) return;

        done_count_ = 0;
        send_cookie(peers_.first, peers_.second);
        send_cookie(peers_.second, peers_.first);
    });
}

void hole_puncher_t::send_cookie(peer_t& sender, peer_t& receiver)
{
    do_send(sender, receiver, sender.cookie_out, [&sender, &receiver, this]()
    {
        if(++done_count_ != 2) return;
        recv_cookie(sender, receiver);
        recv_cookie(receiver, sender);
    });
}

void hole_puncher_t::recv_cookie(peer_t& receiver, peer_t& sender)
{
    do_recv(receiver, sender, [&receiver, &sender, this]()
    {
        if(response_cookie_sent_) {
            finish(hole_t(
                { receiver.handle->public_ip(), receiver.port }
              , { sender.handle->public_ip(),   sender.port   }
            ));
        }
        else send_response_cookie(receiver, sender);
    });
}

void hole_puncher_t::send_response_cookie(peer_t& sender, peer_t& receiver)
{
    static const std::string response_cookie = "RESPONSE_COOKIE";
    do_send(sender, receiver, response_cookie, [&sender, &receiver, this]()
    {
        response_cookie_sent_ = true;
    });
    sender.cookie_out = response_cookie;
}

void hole_puncher_t::do_bind(peer_handle_t& ph, uint16_t port, const std::function<void()>& cb)
{
    ph.bind({port},
            [&ph, cb, port, this]
            (std::vector<std::pair<uint16_t, peer_handle_t::error_code_t>> result)
    {
        if(result.size() != 1)
            return finish(peer_error_t(ph.name(), peer_error_t::EC_PROTOCOL_VIOLATION));
        if(result.back().first != port)
            return finish(peer_error_t(ph.name(), peer_error_t::EC_PROTOCOL_VIOLATION));
        peer_handle_t::error_code_t ec = result.back().second;
        if(ec)
            return finish(peer_error_t(ph.name(), peer_error_t::EC_TOO_MANY_ERRORS));
        cb();
    });
}

void hole_puncher_t::do_recv(peer_t& receiver, peer_t& sender,
                             const std::function<void()>& cb)
{
    receiver.handle->recv({ receiver.port },
        [&receiver, &sender, cb, this]
        (peer_handle_t::error_code_t ec, packet_t packet)
    {
        if(packet.dst_port != receiver.port)
            return finish(peer_error_t(receiver.handle->name(), peer_error_t::EC_PROTOCOL_VIOLATION));
        if(ec)
            return finish(peer_error_t(receiver.handle->name(), peer_error_t::EC_TOO_MANY_ERRORS));
        if(packet.addr != sender.handle->public_ip() ||
           packet.payload != sender.cookie_out)
        {
            // Garbage received. Retry
            return do_recv(receiver, sender, cb);
        }
        cb();
    });
}

void hole_puncher_t::do_send(peer_t& sender, peer_t& receiver,
                          std::string payload, const std::function<void()>& cb)
{
    sender.handle->send(
        { { sender.port
            , receiver.port
            , receiver.handle->public_ip()
            , std::move(payload)
        } },
        [&sender, &receiver, cb, this]
        (std::vector<std::pair<uint16_t, peer_handle_t::error_code_t>> result)
    {
        if(result.size() != 1)
            return finish(peer_error_t(sender.handle->name(), peer_error_t::EC_PROTOCOL_VIOLATION));
        if(result.back().first != sender.port)
            return finish(peer_error_t(sender.handle->name(), peer_error_t::EC_PROTOCOL_VIOLATION));
        peer_handle_t::error_code_t ec = result.back().second;
        if(ec)
            return finish(peer_error_t(sender.handle->name(), peer_error_t::EC_TOO_MANY_ERRORS));
        cb();
    });
}

void hole_puncher_t::stop_peers()
{
    if(peers_.first.handle) {
        peers_.first.handle->stop();
        peers_.first.handle = nullptr;
    }
    if(peers_.second.handle) {
        peers_.second.handle->stop();
        peers_.second.handle = nullptr;
    }
}

void hole_puncher_t::do_finish(const std::shared_ptr<result_t>& result)
{
    if(!result->cb) return;

    cb_t cb;
    cb.swap(result->cb);
    if(result->ex)
        cb(result->ex, hole_t());
    else
        cb(nullptr, std::move(result->hole));
}

void hole_puncher_t::finish(std::exception&& ex)
{
    stop_peers();
    result_->ex = std::make_exception_ptr(std::move(ex));
    context_.event_loop->post(std::bind(&hole_puncher_t::do_finish, result_));
}

void hole_puncher_t::finish(hole_t&& hole)
{
    stop_peers();
    result_->hole = std::move(hole);
    context_.event_loop->post(std::bind(&hole_puncher_t::do_finish, result_));
}

void hole_puncher_t::stop()
{
    stop_peers();
    result_->cb = nullptr;
    result_ = nullptr;
}

} // namespace udp4
} // namespace cpp2p
