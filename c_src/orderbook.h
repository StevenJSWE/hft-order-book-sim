#include <stdint.h> 

typedef struct Order {
    uint64_t id;        // Unique order ID
    uint64_t trader_id; // Who you are
    uint32_t side;      // 0 for Buy (Bid), 1 for Sell (Ask)
    uint64_t price;     // Integer price (e.g., 10005 for $100.05)
    uint64_t quantity;  // How much to buy/sell
    int32_t next;      // Array index of the next order (-1 for end of line)
} Order;

typedef struct PriceLevel {
    uint64_t price;     // The price for this line (e.g., 10000)
    int32_t head;       // Array index of the first order (oldest)
    int32_t tail;       // Array index of the last order (newest)
} PriceLevel;

#define MAX_ORDERS 1000000
#define MAX_PRICE_LEVELS 1000
typedef struct OrderBook{
    Order order_pool[MAX_ORDERS]; // Our pre-allocated array of 1 million empty orders
    int32_t free_head;            // Array index of the first available empty slot 

    // The "bids" (Buy orders) - sorted highest to lowest
    PriceLevel bids[MAX_PRICE_LEVELS];
    int32_t bid_count;

    // The "asks" (Sell orders) - sorted lowest to highest
    PriceLevel asks[MAX_PRICE_LEVELS];
    int32_t ask_count; 

} OrderBook;



// --- ADD THESE THREE LINES BELOW ---
void init_orderbook(OrderBook* ob);
int32_t add_order(OrderBook* ob, uint64_t id, uint64_t trader_id, uint32_t side, uint64_t price, uint64_t quantity);
void process_order(OrderBook* ob, uint64_t id, uint64_t trader_id, uint32_t side, uint64_t price, uint64_t quantity);