/*
 *  peer_handle.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 02/05/2016.
 *
 */

#ifndef CPP2P_UDP4_PEER_HANDLE_H
#define CPP2P_UDP4_PEER_HANDLE_H

#include <stdint.h>

#include <functional>
#include <memory>

#include <boost/range/iterator_range.hpp>

namespace cpp2p {
namespace udp4 {

using port_t = uint16_t;
using ports_t = std::vector<port_t>;

struct packet_t
{
    std::string payload;
    port_t src_port;
    port_t dst_port;
};
using packets_t = std::vector<packet_t>;

class peer_handle_t
{
public:
    enum class error_code_t
    {
        BAD_PAYLOAD_SIZE,
        ADDRESS_ALREADY_IN_USE,
        UNEXPECTED_ERROR
    };
    class delegate_t
    {
    public:
        virtual ~delegate_t() = default;
        virtual void bound(port_t) = 0;
        virtual void error_binding(port_t, error_code_t) = 0;
        virtual void packet_received(const packet_t&) = 0;
        virtual void error_receiving_packet(port_t, error_code_t) = 0;
        virtual void packet_sent(port_t src_port) = 0;
        virtual void error_sending_packet(port_t src_port, error_code_t) = 0;
        virtual void closed(port_t) = 0;
        virtual void error_closing(port_t, error_code_t) = 0;
        virtual void fatal_error_occurred() = 0;
    };

    virtual ~peer_handle_t() = default;
    virtual void set_delegate(std::shared_ptr<delegate_t>) = 0;
    virtual void bind(const ports_t&) = 0;
    virtual void recv(const ports_t&) = 0;
    virtual void send(const packets_t&) = 0;
    virtual void close(const ports_t&) = 0;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_PEER_HANDLE_H
