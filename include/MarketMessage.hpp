#pragma once
#include <cstdint>
#include <string>

namespace market {

enum class MessageType : uint8_t {
    TRADE = 1,
    QUOTE = 2,
    ORDER = 3,
    HEARTBEAT = 4
};

#pragma pack(push, 1)
struct MarketMessage {
    uint64_t timestamp;     // Nanoseconds since epoch
    MessageType type;       
    char symbol[8];         // Fixed-size symbol
    double price;
    uint32_t quantity;
    uint32_t order_id;     // Unique order identifier
};
#pragma pack(pop)

// For efficient binary-level processing
static_assert(sizeof(MarketMessage) == 32, "MarketMessage must be 32 bytes");

} // namespace market