#include "orderbook.h"

void init_orderbook(OrderBook* ob){
    ob->free_head = 0; // Starting at the first slot 

    for(int32_t i = 0; i < MAX_ORDERS; i++){
        ob->order_pool[i].id = 0;
        ob->order_pool[i].price = 0;
        ob->order_pool[i].quantity = 0;

        // Link this slot to the next one in the array 
        if (i < MAX_ORDERS - 1){
            ob->order_pool[i].next = i + 1; 
        } else {
            ob->order_pool[i].next = -1; // Last slot in the pool
        }
    }
}

#include <string.h> // for memmove

// Returns the index of the price level, or creates one if it doesn't exist
int32_t find_or_create_level(OrderBook* ob, uint64_t price, int side) {
    PriceLevel* levels = (side == 0) ? ob->bids : ob->asks;
    int32_t* count = (side == 0) ? &ob->bid_count : &ob->ask_count;

    // 1. Simple Binary Search to find the price
    int low = 0, high = *count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (levels[mid].price == price) return mid;
        
        // Bids are sorted High -> Low, Asks are Low -> High
        if (side == 0) { // Bids
            if (levels[mid].price < price) high = mid - 1;
            else low = mid + 1;
        } else { // Asks
            if (levels[mid].price > price) high = mid - 1;
            else low = mid + 1;
        }
    }

    // 2. If not found, 'low' is the index where it SHOULD be inserted
    int32_t insert_idx = low;

    // Shift existing levels to make room (The "Slow" part of arrays)
    if (insert_idx < *count) {
        memmove(&levels[insert_idx + 1], &levels[insert_idx], sizeof(PriceLevel) * (*count - insert_idx));
    }

    // Initialize the new Price Level
    levels[insert_idx].price = price;
    levels[insert_idx].head = -1;
    levels[insert_idx].tail = -1;
    (*count)++;

    return insert_idx;
}

int32_t add_order(OrderBook* ob, uint64_t id, uint64_t trader_id, uint32_t side, uint64_t price, uint64_t quantity) {
    // 1. Get a slot from the memory pool
    if (ob->free_head == -1) return -1; // Out of memory!

    int32_t order_idx = ob->free_head;
    ob->free_head = ob->order_pool[order_idx].next; // Move free_head to the next empty slot

    // 2. Initialize the order
    Order* o = &ob->order_pool[order_idx];
    o->id = id;
    o->trader_id = trader_id;
    o->side = side;
    o->price = price;
    o->quantity = quantity;
    o->next = -1;

    // 3. Find the price level and attach the order
    int32_t lvl_idx = find_or_create_level(ob, price, side);
    PriceLevel* lvl = (side == 0) ? &ob->bids[lvl_idx] : &ob->asks[lvl_idx];

    if (lvl->head == -1) {
        // First order at this price
        lvl->head = order_idx;
        lvl->tail = order_idx;
    } else {
        // Attach to the end of the line (FIFO)
        ob->order_pool[lvl->tail].next = order_idx;
        lvl->tail = order_idx;
    }

    return order_idx;
}

void process_order(OrderBook* ob, uint64_t id, uint64_t trader_id, uint32_t side, uint64_t price, uint64_t quantity) {
    // 1. Try to match against existing orders
    while (quantity > 0) {
        // If buying, look at best ask (asks[0]). If selling, look at best bid (bids[0]).
        int32_t* opp_count = (side == 0) ? &ob->ask_count : &ob->bid_count;
        PriceLevel* opp_levels = (side == 0) ? ob->asks : ob->bids;

        if (*opp_count == 0) break; // No one to trade with

        PriceLevel* best_opp = &opp_levels[0];

        // Check if prices cross
        // (If buying, price must be >= best ask. If selling, price must be <= best bid.)
        if ((side == 0 && price < best_opp->price) || (side == 1 && price > best_opp->price)) {
            break; 
        }

        // 2. Match against the head of the best price level
        Order* matching_order = &ob->order_pool[best_opp->head];
        uint64_t trade_qty = (quantity < matching_order->quantity) ? quantity : matching_order->quantity;

        // Execute trade (In a real system, you'd log this or send to Go here)
        quantity -= trade_qty;
        matching_order->quantity -= trade_qty;

        // 3. If the matching order is fully filled, remove it
        if (matching_order->quantity == 0) {
            int32_t filled_idx = best_opp->head;
            best_opp->head = matching_order->next;

            // Return filled_idx to memory pool
            matching_order->next = ob->free_head;
            ob->free_head = filled_idx;

            // If the price level is now empty, remove the level
            if (best_opp->head == -1) {
                // Shift all levels down by 1
                memmove(&opp_levels[0], &opp_levels[1], sizeof(PriceLevel) * (*opp_count - 1));
                (*opp_count)--;
            }
        }
    }

    // 4. If there is still quantity left, add the remaining to the book
    if (quantity > 0) {
        add_order(ob, id, trader_id, side, price, quantity);
    }
}