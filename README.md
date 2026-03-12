# HFT Order Book Simulation (C + Go)

A high-frequency trading (HFT) matching engine built for speed. The core matching logic and memory management are written in C for zero-latency execution, while a Go web server handles the API and routing via CGO.

## 🧠 What I Learned & Built

This project was a deep dive into low-level performance and cross-language integration:

- **Zero-Allocation Memory Pools (C):** Instead of using slow `malloc` calls for every new trade, the engine pre-allocates an array of 1,000,000 order slots at startup. Orders are recycled through a linked list, making the matching engine incredibly fast and memory-safe.
- **The CGO Bridge (C + Go):** Successfully linked low-level C structs and functions directly into a high-level Go web server. Navigated compilation flags, header guards, and type-casting between C and Go types.
- **Cloud Deployment (Docker + Railway):** Wrote a custom Dockerfile to compile the C code (`gcc`) alongside the Go code (`CGO_ENABLED=1`) and successfully deployed the hybrid binary to a Railway production server.
- **Live State Management:** Served real-time data from a C memory struct directly to a web browser and JSON API without crashing or leaking memory.

## 🛠️ Tech Stack

- **Engine:** C (Custom memory pooling, pointer manipulation)
- **API/Web:** Go (net/http, CGO)
- **Deployment:** Docker, Railway
- **Testing:** Python (Requests loop for stress testing)

## 🚀 How to Run It

### Local Go Server

To run the server locally on your machine:

```bash
go build -o engine .
./engine
```
