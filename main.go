package main

/*
#cgo CFLAGS: -I./c_src
#include "orderbook.h"

// We declare the book here so Go manages the lifecycle
OrderBook book;
*/
import "C"
import (
	"fmt"
	"time"
)

func main() {
	// 1. Initialize the C Engine
	C.init_orderbook(&C.book)
	fmt.Println("🚀 HFT Engine Initialized")

	start := time.Now()
	
	// 2. Simulate 100,000 matches (200,000 orders)
	count := 100000
	for i := 0; i < count; i++ {
		// Trader 1 Sells at 100
		C.process_order(&C.book, C.uint64_t(i), 1, 1, 100, 10)
		// Trader 2 Buys at 100 (Matches immediately)
		C.process_order(&C.book, C.uint64_t(i+count), 2, 0, 100, 10)
	}

	duration := time.Since(start)
	
	fmt.Printf("✅ Processed %d orders in %v\n", count*2, duration)
	fmt.Printf("⚡ Speed: %.0f orders/sec\n", float64(count*2)/duration.Seconds())
}