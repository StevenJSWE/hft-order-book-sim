#include <stdio.h>
#include "orderbook.h"

OrderBook myBook;

int main() {
    // 1. Setup the book
    init_orderbook(&myBook);
    myBook.bid_count = 0;
    myBook.ask_count = 0;

    printf("--- HFT Engine Local Test ---\n");

    // 2. Add a Limit Sell Order (Ask)
    // Trader 1 wants to sell 100 units at $10.50 (1050)
    printf("Adding Sell Order: 100 units @ 1050\n");
    process_order(&myBook, 101, 1, 1, 1050, 100);

    // 3. Add a Limit Buy Order (Bid) that matches it
    // Trader 2 wants to buy 100 units at $10.50 (1050)
    printf("Adding Buy Order: 100 units @ 1050\n");
    process_order(&myBook, 102, 2, 0, 1050, 100);

    // 4. Check results
    printf("\n--- Results ---\n");
    printf("Active Bids: %d\n", myBook.bid_count);
    printf("Active Asks: %d\n", myBook.ask_count);
    
    if (myBook.bid_count == 0 && myBook.ask_count == 0) {
        printf("SUCCESS: The orders matched and cancelled each other out!\n");
    } else {
        printf("FAILURE: Orders are still sitting in the book.\n");
    }

    return 0;
}