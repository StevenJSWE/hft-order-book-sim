package main

/*
#cgo CFLAGS: -I./c_src
// Note: We REMOVED the #include "orderbook.c" and the LDFLAGS here
#include "orderbook.h"

extern OrderBook book;
*/
import "C"

// We move the variable declaration to the C file to avoid Go/C link confusion
func main() {
    // ... rest of your code ...
}