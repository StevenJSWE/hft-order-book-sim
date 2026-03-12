package main

/*
#cgo CFLAGS: -I./c_src
#include "c_src/orderbook.h"
#include "c_src/orderbook.c"

extern OrderBook book;
*/
import "C"
import (
	"encoding/json"
	"fmt"
	"net/http"
	"os"
)

// OrderRequest matches the JSON we will send to the API
type OrderRequest struct {
	ID       uint64 `json:"id"`
	TraderID uint64 `json:"trader_id"`
	Side     uint32 `json:"side"` // 0 = Buy, 1 = Sell
	Price    uint64 `json:"price"`
	Quantity uint64 `json:"quantity"`
}

func main() {
	C.init_orderbook(&C.book)

	// A few resting orders so the book isn't empty
	C.process_order(&C.book, C.uint64_t(1), C.uint64_t(101), 1, 110, 50)
	C.process_order(&C.book, C.uint64_t(2), C.uint64_t(201), 0, 90, 40)

	// Route 1: The UI Dashboard
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/html")
		fmt.Fprintf(w, "<h2>🚀 HFT Engine Live</h2>")
		fmt.Fprintf(w, "<p>Bids: %d | Asks: %d</p>", int(C.book.bid_count), int(C.book.ask_count))
		fmt.Fprintf(w, "<p>Status: <b>Engine Healthy (Zero-Allocation Pool)</b></p>")
	})

	// Route 2: The Trading API
	http.HandleFunc("/order", func(w http.ResponseWriter, r *http.Request) {
		if r.Method != http.MethodPost {
			http.Error(w, "Only POST allowed", http.StatusMethodNotAllowed)
			return
		}

		var req OrderRequest
		if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
			return
		}

		// Fire it directly into the C Engine
		C.process_order(
			&C.book,
			C.uint64_t(req.ID),
			C.uint64_t(req.TraderID),
			C.uint32_t(req.Side),
			C.uint64_t(req.Price),
			C.uint64_t(req.Quantity),
		)

		// Return the new book state
		w.Header().Set("Content-Type", "application/json")
		fmt.Fprintf(w, `{"status": "success", "bids": %d, "asks": %d}`, int(C.book.bid_count), int(C.book.ask_count))
	})

	port := os.Getenv("PORT")
	if port == "" {
		port = "8080"
	}

	fmt.Printf("🌍 Server starting on port %s\n", port)
	http.ListenAndServe(":"+port, nil)
}