import requests
import time

# Update this to your live Railway URL
BASE_URL = "https://hft-order-book-sim-production.up.railway.app/"
# BASE_URL = "http://localhost:8080" # Uncomment for local testing

def send_order(order_id, trader_id, side, price, quantity):
    """
    Sends an order to the HFT engine.
    Side: 0 = Buy (Bid), 1 = Sell (Ask)
    """
    url = f"{BASE_URL}/order"
    payload = {
        "id": order_id,
        "trader_id": trader_id,
        "side": side,
        "price": price,
        "quantity": quantity
    }
    
    side_str = "Sell" if side == 1 else "Buy "
    print(f"📤 Sending {side_str} | Price: {price} | Qty: {quantity}...")
    
    try:
        response = requests.post(url, json=payload)
        response.raise_for_status()
        print(f"✅ Success! Engine State: {response.json()}\n")
    except requests.exceptions.RequestException as e:
        print(f"❌ Error: {e}\n")

if __name__ == "__main__":
    print(f"--- HFT Engine Stress Test ---")
    print(f"Targeting: {BASE_URL}\n")

    # 1. Add a resting Buy order at $100
    send_order(order_id=1000, trader_id=10, side=0, price=100, quantity=50)
    time.sleep(0.5)

    # 2. Add a resting Sell order at $110 (Doesn't match, book grows)
    send_order(order_id=1001, trader_id=20, side=1, price=110, quantity=50)
    time.sleep(0.5)

    # 3. Send a Sell order at $100 (Matches the Buy! Book size should shrink)
    send_order(order_id=1002, trader_id=30, side=1, price=100, quantity=50)

    print("Test complete. Refresh your browser to see the final resting state.")