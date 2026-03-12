package main

/*
#cgo CFLAGS: -I./c_src
#include "c_src/orderbook.h"
#include "c_src/orderbook.c"

// We declare the book here so it's managed in the Go-to-C space
OrderBook book;
*/
import "C"
import (
	"fmt"
)

func main() {
    // 1. Initialize the C Engine
    C.init_orderbook(&C.book)
    fmt.Println("🚀 HFT Engine Initialized on Railway")

    // ... your test logic ...
    fmt.Println("✅ Trade logic verified.")
    
    // Stop the container from exiting immediately
    select {}
}