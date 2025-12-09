**AQI MONITORING SYSTEM**

<img width="400" height="400" alt="AQI" src="https://github.com/user-attachments/assets/f52feb1c-05b7-4b58-8ca2-de2db5cb565b" />

🎥 **Watch Demo Video:**  
[https://www.linkedin.com/posts/AQI SYSTEM](https://www.linkedin.com/posts/utkarsha-avinash-98b984374_iot-esp32-airquality-activity-7404236930810408960-rZ-l/)

## 📘 **About the Project**

This project is a **basic AQI monitoring system** built using:

* **ESP32 DevKit V1**
* **Sharp GP2Y1010 PM2.5 Dust Sensor**
* **MQ Smoke/Gas Sensor**
* **DHT22 Temperature/Humidity Sensor**
* **5V & 3.3V regulators**

It collects real-time atmospheric data and displays everything on a **mobile-friendly web dashboard** hosted directly on the ESP32.

### 🎯 **Approx. Accuracy**

| Sensor               | Accuracy                                   |
| -------------------- | ------------------------------------------ |
| **DHT22**            | ±0.5°C, ±2–3% RH                           |
| **PM2.5 (GP2Y1010)** | ~60–75% accurate indoors (with correction) |
| **MQ Gas**           | qualitative smoke detection                |

This system is ideal for:

* School/college projects
* Personal indoor AQI monitoring
* Demonstration prototypes
* Microcontroller learning

---

## ⚙️ **How It Works**

### 1️⃣ **Sensor Readings**

ESP32 continuously reads:

* PM2.5 (dust particles)
* AQI (calculated using EPA standards)
* Temperature
* Humidity
* Smoke/Gas levels

### 2️⃣ **ESP32 as Wi-Fi Hotspot**

The ESP32 creates its own Wi-Fi network:

```
SSID:     MyESP  
Password: 12345678
```

### 3️⃣ **Live Dashboard**

Connect to ESP32 and open:

👉 **[http://192.168.4.1/](http://192.168.4.1/)**

You will see:

* Large, color-changing AQI card
* PM2.5 in µg/m³
* Temperature
* Humidity
* Smoke indicator

UI updates every second.

### 🎨 **UI Note**

The dashboard UI was refined and styled with the help of **AI tools like ChatGPT**, making it clean, responsive, and mobile-friendly.

---

## 🧩 **Components Used**

| Component                    | Function                   |
| ---------------------------- | -------------------------- |
| ESP32 DevKit V1              | WiFi + processing          |
| GP2Y1010 PM Sensor           | PM2.5 detection            |
| DHT22                        | Temperature + humidity     |
| MQ Sensor                    | Smoke/Gas detection        |
| 150Ω resistor                | GP2Y1010 LED current limit |
| 220µF electrolytic capacitor | PM sensor noise filtering  |
| 5V & 3.3V regulators         | Stable power supply        |
| Common GND                   | Shared reference           |

---

## 📐 **Circuit Diagram**

<img width="1114" height="594" alt="AQI_Circuit" src="https://github.com/user-attachments/assets/55a2cc59-68bd-428c-af34-350474ace03b" />


---

## 📂 **Downloadable Project Files**

### 💾 **Code (INO file)**

[📄 Download AQIRead.ino](AQIRead.ino)


### 📦 **Complete ZIP Project**

[📄 Download AQIRead.ino](AQIALL.zip)


---

## 🚀 **How to Run**

1. Power system with 5V
2. ESP32 hotspot appears
3. Connect:

   * SSID: `MyESP`
   * Password: `12345678`
4. Open browser → **[http://192.168.4.1/](http://192.168.4.1/)**
5. Monitor AQI in real-time 🎉

---

## 👨‍💻 **Developer**

**Utkarsha Avinash**      

🔗 **LinkedIn:**
`www.linkedin.com/in/utkarsha-avinash-98b984374`

---

