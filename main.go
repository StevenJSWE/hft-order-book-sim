package main

/*
#cgo CFLAGS: -I./c_src
#include "c_src/orderbook.h"
#include "c_src/orderbook.c"

// This matches the OrderBook struct in your C code
OrderBook book;
*/
import "C"
import (
	"fmt"
	"net/http"
	"os"
)

func main() {
	// 1. Initialize the C Engine memory pool
	C.init_orderbook(&C.book)

	// 2. Add some "Resting" orders that won't match immediately
	// Format: book, id, trader_id, side (0=buy, 1=sell), price, quantity
	
	// Two Sellers (Asks) - Higher prices
	C.process_order(&C.book, C.uint64_t(1), C.uint64_t(101), 1, 110, 50)
	C.process_order(&C.book, C.uint64_t(2), C.uint64_t(102), 1, 115, 25)

	// Two Buyers (Bids) - Lower prices
	C.process_order(&C.book, C.uint64_t(3), C.uint64_t(201), 0, 90, 40)
	C.process_order(&C.book, C.uint64_t(4), C.uint64_t(202), 0, 85, 100)

	fmt.Println("🚀 HFT Engine Logic Loaded. Orders resting in memory.")

	// 3. Setup Web Dashboard
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/html")
		fmt.Fprintf(w, "<body style='font-family: sans-serif; padding: 40px; line-height: 1.6;'>")
		fmt.Fprintf(w, "<h1 style='color: #2c3e50;'>🚀 HFT Engine Live</h1>")
		fmt.Fprintf(w, "<div style='background: #f4f7f6; padding: 20px; border-radius: 8px;'>")
		fmt.Fprintf(w, "<h3>Order Book Depth</h3>")
		fmt.Fprintf(w, "<p><strong>Active Bids (Buyers):</strong> %d</p>", int(C.book.bid_count))
		fmt.Fprintf(w, "<p><strong>Active Asks (Sellers):</strong> %d</p>", int(C.book.ask_count))
		fmt.Fprintf(w, "</div>")
		fmt.Fprintf(w, "<p style='color: #27ae60;'>Status: <strong>Engine Healthy (Zero-Allocation Pool)</strong></p>")
		fmt.Fprintf(w, "<small>Note: Refresh the page to see live state.</small>")
		fmt.Fprintf(w, "</body>")
	})

	// 4. Railway Port Logic
	port := os.Getenv("PORT")
	if port == "" {
		port = "8080"
	}

	fmt.Printf("🌍 Dashbaord active at: http://localhost:%s\n", port)
	err := http.ListenAndServe(":"+port, nil)
	if err != nil {
		fmt.Printf("Error starting server: %v\n", err)
	}
}