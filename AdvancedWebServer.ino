#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "TP-Link_4AA4";
const char* password = "Shubham@123";

// Set web server port number to 80
WiFiServer server(80);

// Authentication credentials
const char* authUser = "admin";
const char* authPass = "password";

// Variable to store the HTTP request
String header;

// GPIO output states
String outputD1State = "off";
String outputD2State = "off";

// Assign output variables to GPIO pins
const int outputD1 = D1;  // GPIO 5
const int outputD2 = D2;  // GPIO 4

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// Function for Base64 encoding
String base64Encode(String input) {
  const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String encodedString;
  int i = 0;
  unsigned char array3[3];
  unsigned char array4[4];

  for (unsigned int j = 0; j < input.length(); j++) {
    array3[i++] = input[j];
    if (i == 3) {
      array4[0] = (array3[0] & 0xfc) >> 2;
      array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
      array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
      array4[3] = array3[2] & 0x3f;

      for (i = 0; i < 4; i++)
        encodedString += base64Chars[array4[i]];

      i = 0;
    }
  }

  if (i > 0) {
    for (int j = i; j < 3; j++) array3[j] = '\0';

    array4[0] = (array3[0] & 0xfc) >> 2;
    array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
    array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
    array4[3] = array3[2] & 0x3f;

    for (int j = 0; j < i + 1; j++) encodedString += base64Chars[array4[j]];

    while (i++ < 3) encodedString += '=';
  }

  return encodedString;
}

void setup() {
  Serial.begin(115200);
  pinMode(outputD1, OUTPUT);
  pinMode(outputD2, OUTPUT);
  digitalWrite(outputD1, LOW);
  digitalWrite(outputD2, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";
    bool authenticated = false;

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Check for Authorization Header
            if (header.indexOf("Authorization: Basic") >= 0) {
              int authStart = header.indexOf("Authorization: Basic") + 21;
              String encodedAuth = header.substring(authStart, header.indexOf('\n', authStart));
              encodedAuth.trim();
              String expectedAuth = base64Encode(String(authUser) + ":" + String(authPass));
              if (encodedAuth.equals(expectedAuth)) {
                authenticated = true;
              }
            }

            if (authenticated) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if (header.indexOf("GET /D1/on") >= 0) {
                Serial.println("GPIO D1 on");
                outputD1State = "on";
                digitalWrite(outputD1, HIGH);
              } else if (header.indexOf("GET /D1/off") >= 0) {
                Serial.println("GPIO D1 off");
                outputD1State = "off";
                digitalWrite(outputD1, LOW);
              } else if (header.indexOf("GET /D2/on") >= 0) {
                Serial.println("GPIO D2 on");
                outputD2State = "on";
                digitalWrite(outputD2, HIGH);
              } else if (header.indexOf("GET /D2/off") >= 0) {
                Serial.println("GPIO D2 off");
                outputD2State = "off";
                digitalWrite(outputD2, LOW);
              }

              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<style>html { font-family: Helvetica; text-align: center;} .button { background-color: #4CAF50; color: white; padding: 16px 40px; margin: 4px; cursor: pointer;} .button2 {background-color: #555555;}</style></head>");
              client.println("<body><h1>ESP8266 Web Server</h1>");

              client.println("<p>GPIO D1 - State " + outputD1State + "</p>");
              client.println("<p><a href=\"/D1/on\"><button class=\"button\">ON</button></a>");
              client.println("<a href=\"/D1/off\"><button class=\"button button2\">OFF</button></a></p>");

              client.println("<p>GPIO D2 - State " + outputD2State + "</p>");
              client.println("<p><a href=\"/D2/on\"><button class=\"button\">ON</button></a>");
              client.println("<a href=\"/D2/off\"><button class=\"button button2\">OFF</button></a></p>");

              client.println("</body></html>");
              client.println();
            } else {
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"ESP8266\"");
              client.println("Connection: close");
              client.println();
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}
