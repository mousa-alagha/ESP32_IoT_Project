# ESP32 IoT Project

An IoT project that collects temperature, humidity, and vibration data using ESP32, sending it to the Blynk app and MQTT broker. Includes remote LED control.

## How to Use
1. Install the **Arduino IDE** if you don't have it.
2. Install the **ESP32 Board** in the Arduino IDE.
3. Open the `ESP32_IoT_Project.ino` file in the Arduino IDE.
4. Replace the placeholders in the code with your credentials (WiFi, Blynk, MQTT).
5. Upload the code to your ESP32 board.
6. Monitor the data in the Blynk app and control the LED remotely.

## Requirements:
- ESP32 board
- DHT11 sensor for temperature and humidity
- ADXL345 accelerometer
- Blynk app on your mobile
- MQTT broker
