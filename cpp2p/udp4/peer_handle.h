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
    using port_cb_t = std::function<void(error_code_t, port_t)>;
    using packet_cb_t = std::function<void(error_code_t, port_t src, port_t dst,
                                           std::string payload)>;

    virtual void bind(const ports_t&, const port_cb_t&) = 0;
    virtual void recv(const ports_t&, const packet_cb_t&) = 0;
    virtual void send(port_t src, port_t dst, const std::string& payload,
                      const port_cb_t&) = 0;
    virtual void close(const ports_t&, const port_cb_t&) = 0;
    virtual void stop() = 0;
};

} // namespace udp4
} // namespace cpp2p

#endif // CPP2P_UDP4_PEER_HANDLE_H
