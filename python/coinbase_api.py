import os
import json, requests

import cbpro

if __name__ == "__main__":
    api_url = "https://api-public.sandbox.pro.coinbase.com"
    key_file = os.getenv("CB_KEY_FILE")
    print(key_file)
    with open(key_file) as f:
        key_data = json.loads(f.read())
    print(key_data)
    auth_client = cbpro.AuthenticatedClient(
        key_data["key"], key_data["secret"],
        key_data["passphrase"], api_url=api_url)

    r = auth_client.get_accounts()
    print(r)

    buy_order = {
        'size': 1.0,
        'price': 0.01,
        'product_id': 'BTC-USD',
        "order_type": "limit"
    }
    r = auth_client.buy(**buy_order)
    print(r)

    sell_order = {
        'size': 1.0,
        'price': 100000,
        'product_id': 'BTC-USD',
        "order_type": "limit"
    }
    r = auth_client.sell(**sell_order)
    print(r)
