import requests
import random
import time

BASE_URL = "https://hft-order-book-sim-production.up.railway.app"

print("🚀 Initiating MENTAL stress test... Press Ctrl+C to stop.")

order_id = 1000
while True:
    # Keep prices between 95 and 105 so buyers and sellers constantly collide!
    side = random.choice([0, 1])
    price = random.randint(95, 105) 
    quantity = random.randint(1, 50)
    
    payload = {
        "id": order_id,
        "trader_id": random.randint(1, 100),
        "side": side,
        "price": price,
        "quantity": quantity
    }
    
    try:
        # Fire the order
        res = requests.post(f"{BASE_URL}/order", json=payload)
        data = res.json()
        
        # Print the live carnage to the terminal
        side_str = "🔴 SELL" if side == 1 else "🟢 BUY "
        print(f"{side_str} {quantity:2} @ ${price:<3} | Book -> Bids: {data['bids']:<4} Asks: {data['asks']}")
        
    except Exception as e:
        print(f"❌ Connection error: {e}")
        
    order_id += 1
    
    # Sleep for 50 milliseconds (20 requests per second)
    time.sleep(0.05)