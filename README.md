# PBL 5: Embedded Systems

## 1. Overview

This repository contains the **PBL5Hethongnhung** project for the course *PBL 5: Embedded Systems*.  
It is **adapted from Espressif’s ESP32‑C3 IoT Projects examples** to work with **ESP‑IDF v5.5**.  

> All original code is from Espressif; modifications were made to make it compatible with ESP‑IDF v5.5 and for educational purposes.

---

## 2. Project Structure

```
├── device_firmware/
│   ├── 1_blink/
│   ├── 2_light_drivers/
│   ├── 3_wifi_connection/
│   ├── 4_network_config/
│   ├── 5_rainmaker/
│   ├── 6_project_optimize/
│   ├── 7_insights/
│   └── components/
│       ├── app_storage/
│       ├── app_wifi/
│       ├── button/
│       └── light_driver/
├── phone_app/
│   └── app_android/
├── test_case/
│   ├── gatt_server/
│   ├── local_control/
│   ├── mqtt_tcp/
│   └── tcp_socket/
└── README.md
```

---

## 3. Requirements

- **ESP‑IDF v5.5** (or newer)  
- ESP32‑C3 toolchain installed  
- Python for ESP‑IDF tools  

Follow Espressif’s official setup guide before building.

---

## 4. Build & Flash

1. Navigate to a firmware example:
	```bash
 	cd device_firmware/1_blink
 	```
 2. Configure project:
	```bash
	idf.py menuconfig
 	```
 3. Build and flash:
	```bash
	idf.py build
 	idf.py flash
 	```
 4. Monitor output:
	```bash
	idf.py monitor
 	```
 
 > Replace `1_bink` with the example you want to run.

 ## 5. References

 * ESP32-C3 Wireless Adventure PDF
 * Espressif ESP32-C3 Book (English)

> Original examples are from Espresssif. This repository is adapted for learning and PBL 5 purposes.
