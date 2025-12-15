Thay đổi LIGHT_TCP_CLIENT

* LIGHT_TCP_CLIENT = 1 (ESP32 là Client)
1. Terminal 1: Chạy Python Server
python server.py
2. Terminal 2: Chạy ESP-IDF

* LIGHT_TCP_CLIENT = 0 (ESP32 là Server)
1. Terminal 1: Chạy ESP-IDF
2. Terminal 2: Chạy Python Client
python client.py
