package main

/*
#cgo CFLAGS: -I./c_src
#include "c_src/orderbook.h"
#include "c_src/orderbook.c"

OrderBook book;
*/
import "C"
import (
	"fmt"
	"net/http"
	"os"
)

func main() {
	C.init_orderbook(&C.book)

	// Route to see the book status
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintf(w, "<h1>HFT Engine Live</h1>")
		fmt.Fprintf(w, "<p>Bids: %d</p>", int(C.book.bid_count))
		fmt.Fprintf(w, "<p>Asks: %d</p>", int(C.book.ask_count))
		fmt.Fprintf(w, "<hr><p>Status: Engine running at zero-latency</p>")
	})

	// Railway provides a PORT environment variable
	port := os.Getenv("PORT")
	if port == "" {
		port = "8080"
	}

	fmt.Printf("🌍 Server starting on port %s\n", port)
	http.ListenAndServe(":"+port, nil)
}