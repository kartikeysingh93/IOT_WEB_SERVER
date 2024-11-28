#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "TP-Link_4AA4";
const char* password = "Shubham@123";

// Authentication credentials
const char* authUser = "admin";
const char* authPass = "password";

// GPIO pins for controlling
const int outputD1 = D1;  // GPIO 5
const int outputD2 = D2;  // GPIO 4

// Initialize the server on port 80
ESP8266WebServer server(80);

// Function to handle login page request
void handleLogin() {
  String html = "<html><body style='font-family: Arial, sans-serif;'>";
  html += "<div style='text-align: center; padding: 50px;'>";
  html += "<h2 style='color: #4CAF50;'>Login to ESP8266 Web Server</h2>";
  html += "<form action='/login' method='POST' style='margin-top: 20px;'>";
  html += "Username: <input type='text' name='username' style='padding: 10px; margin: 10px; width: 200px;' required><br>";
  html += "Password: <input type='password' name='password' style='padding: 10px; margin: 10px; width: 200px;' required><br>";
  html += "<input type='submit' value='Login' style='background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer;'>";
  html += "</form></div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Function to authenticate user and redirect
void handleLoginPost() {
  String username = server.arg("username");
  String password = server.arg("password");

  // Validate the credentials
  if (username == authUser && password == authPass) {
    // Redirect to the GPIO control page after successful login
    server.sendHeader("Location", "/index", true);
    server.send(303, "text/plain", "");
  } else {
    // If authentication fails, send a 401 Unauthorized response
    server.send(401, "text/html", "<h1 style='text-align:center;'>Unauthorized</h1>");
  }
}

// Function to serve the GPIO control page
void handleIndex() {
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 0; text-align: center; }";
  html += "h2 { color: #4CAF50; margin-top: 20px; }";
  html += "button { background-color: #4CAF50; color: white; padding: 15px 32px; font-size: 16px; margin: 10px; border: none; cursor: pointer; }";
  html += "button:hover { background-color: #45a049; }";
  html += "p { font-size: 20px; color: #333; }";
  html += ".container { max-width: 600px; margin: 20px auto; padding: 20px; background-color: white; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }";
  html += "@media screen and (max-width: 600px) { button { width: 100%; } }";
  html += "</style>";
  html += "</head><body>";

  html += "<div class='container'>";
  html += "<h2>Control GPIO Pins</h2>";
  
  // Display GPIO D1 status and control buttons
  html += "<p>GPIO D1 - State: ";
  html += (digitalRead(outputD1) == HIGH) ? "<strong>ON</strong>" : "<strong>OFF</strong>";
  html += "</p>";
  html += "<a href='/D1/on'><button>Turn ON</button></a>";
  html += "<a href='/D1/off'><button>Turn OFF</button></a>";

  // Display GPIO D2 status and control buttons
  html += "<p>GPIO D2 - State: ";
  html += (digitalRead(outputD2) == HIGH) ? "<strong>ON</strong>" : "<strong>OFF</strong>";
  html += "</p>";
  html += "<a href='/D2/on'><button>Turn ON</button></a>";
  html += "<a href='/D2/off'><button>Turn OFF</button></a>";

  html += "</div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Function to handle turning on GPIO D1
void handleD1On() {
  digitalWrite(outputD1, HIGH);
  server.sendHeader("Location", "/index", true);
  server.send(303, "text/plain", "");
}

// Function to handle turning off GPIO D1
void handleD1Off() {
  digitalWrite(outputD1, LOW);
  server.sendHeader("Location", "/index", true);
  server.send(303, "text/plain", "");
}

// Function to handle turning on GPIO D2
void handleD2On() {
  digitalWrite(outputD2, HIGH);
  server.sendHeader("Location", "/index", true);
  server.send(303, "text/plain", "");
}

// Function to handle turning off GPIO D2
void handleD2Off() {
  digitalWrite(outputD2, LOW);
  server.sendHeader("Location", "/index", true);
  server.send(303, "text/plain", "");
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set GPIO pins as outputs
  pinMode(outputD1, OUTPUT);
  pinMode(outputD2, OUTPUT);
  digitalWrite(outputD1, LOW);
  digitalWrite(outputD2, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP8266 local IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", HTTP_GET, handleLogin);      // Display the login page
  server.on("/login", HTTP_POST, handleLoginPost);  // Handle POST login
  server.on("/index", HTTP_GET, handleIndex);  // Show GPIO control page
  server.on("/D1/on", HTTP_GET, handleD1On);    // Turn on GPIO D1
  server.on("/D1/off", HTTP_GET, handleD1Off);  // Turn off GPIO D1
  server.on("/D2/on", HTTP_GET, handleD2On);    // Turn on GPIO D2
  server.on("/D2/off", HTTP_GET, handleD2Off);  // Turn off GPIO D2

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();
}

