/*
 *  cpp2p/event_loop.h
 *  cpp2p
 *
 *  Created by Oleg Andriyanov on 12/06/2016.
 *
 */

#ifndef CPP2P_EVENT_LOOP_H
#define CPP2P_EVENT_LOOP_H

#include <functional>

namespace cpp2p {

class event_loop_t
{
public:
    virtual void post(const std::function<void()>&) = 0;
};

} // namespace cpp2p

#endif // CPP2P_EVENT_LOOP_H
