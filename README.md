🔌 ESP8266 Web-Based GPIO Controller with Login Authentication

A simple yet functional web-based interface to control GPIO pins (D1 and D2) on an ESP8266 board using Wi-Fi. The interface includes a basic login authentication mechanism to restrict access.

📦 Features
🌐 Web interface served from ESP8266

🔐 Basic login authentication (username + password)

🎛️ Control GPIO D1 and D2 (ON/OFF)

📱 Responsive design for mobile & desktop

⚡ Real-time state reflection for each GPIO

🛠️ Hardware Required
ESP8266 board (e.g., NodeMCU)

Micro-USB cable

Wi-Fi network

Optional: Relay module, LED, etc. for GPIO outputs

📡 Circuit Connections
GPIO	Pin	Purpose
D1	5	Output device 1
D2	4	Output device 2
🔧 Installation
Clone the Repository

bash
Copy
Edit
git clone https://github.com/kartikeysingh93/IOT_WEB_SERVER.git
cd esp8266-gpio-controller
Open in Arduino IDE

Install Required Libraries

ESP8266WiFi

ESP8266WebServer

Edit Credentials in Code

cpp
Copy
Edit
const char* ssid = "Your_SSID";
const char* password = "Your_WIFI_Password";
const char* authUser = "admin";
const char* authPass = "password";
Upload to Board

Open Serial Monitor

Baud Rate: 115200

Note the local IP address printed after connecting to Wi-Fi.

Access the Web Interface

Open a browser and enter the IP shown in Serial Monitor.

🔐 Default Login Credentials
Field	Value
Username	admin
Password	password
🖥️ Web Interface Preview
Login Page

GPIO Control Dashboard

Turn D1 and D2 ON/OFF

Displays real-time GPIO state

🚀 Example Use Cases
Home automation

IoT projects

Remote device control

📜 License
MIT License
