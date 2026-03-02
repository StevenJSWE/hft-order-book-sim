#include <stdint.h> 

typedef struct Order {
    uint64_t id;        // Unique order ID
    uint64_t trader_id; // Who you are
    uint32_t side;      // 0 for Buy (Bid), 1 for Sell (Ask)
    uint64_t price;     // Integer price (e.g., 10005 for $100.05)
    uint64_t quantity;  // How much to buy/sell
    int32_t next;      // Array index of the next order (-1 for end of line)
} Order;