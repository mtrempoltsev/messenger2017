#pragma once

#include <string>

namespace m2 {
namespace routing {

class Message
{
public:
    Message();
    ~Message();
};

enum class MessageType {
    PingRequest,
    StoreRequest,
    FindNodeRequest,
    FindDataRequest,

    PingResponse,
    StoreResponse,
    FindNodeResponse,
    FindDataResponse
};

class MessageTypeHash
{
public:
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

} // namespace routing
} // namespace m2