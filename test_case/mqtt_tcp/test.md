1. Run Mosquitto
mosquitto -v

2. Terminal 1: Subscribe topic
mosquitto_sub -h broker.emqx.io -p 1883 -t "/topic/qos0"

3. Terminal 2: Publish message gửi lên ESP32C3
mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/qos0" -m "hello ESP"