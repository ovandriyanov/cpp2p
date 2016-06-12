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
    port_t src_port;
    port_t dst_port;
    uint32_t addr; // src for incoming packets, dst for outgoing
    std::string payload;
};

using packets_t = std::vector<packet_t>;

class peer_handle_t
{
public:
    enum error_code_t
    {
        EC_OK = 0,
        EC_BAD_PAYLOAD_SIZE,
        EC_ADDRESS_ALREADY_IN_USE,
        EC_UNEXPECTED_ERROR
    };
    using ports_cb_t = std::function<void(std::vector<std::pair<port_t, error_code_t>>)>;
    using packet_cb_t = std::function<void(error_code_t, packet_t)>;

    virtual void bind(const ports_t&, const ports_cb_t&) = 0;
    virtual void recv(const ports_t&, const packet_cb_t& on_receive_cb) = 0;
    virtual void send(const packets_t&, const ports_cb_t&) = 0;
    virtual void close(const ports_t&, const ports_cb_t&) = 0;
    virtual void stop() = 0;

    virtual uint32_t public_ip() const = 0;
    static const uint16_t any_port = 0;
    virtual uint16_t desired_port() const { return any_port; }
    virtual const std::string& name() const { static const std::string unnamed("unnamed"); return unnamed; }
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_PEER_HANDLE_H
