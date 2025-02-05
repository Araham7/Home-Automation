#include <SocketIOclient.h>   // Socket.IO client library
#include <WiFi.h>             // ESP32 Wi-Fi library
#include <ArduinoJson.h>      // ArduinoJson library

#define LED 2

// Wi-Fi Credentials
const char* ssid = "Laptop";
const char* password = "8918172922";

// WebSocket Server Details
const char* host = "esp8266-led-backend.onrender.com";  // Render server
const int port = 443;  // HTTPS/WebSocket Secure Port

SocketIOclient socketIO;  // WebSocket client object

void setup() {
    Serial.begin(9600);  // Start Serial Monitor
    pinMode(LED , OUTPUT);
    digitalWrite(LED , LOW);  // Turn off the light

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n✅ Wi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Connect to WebSocket Server (Use "wss://" for secure connection)
    socketIO.beginSSL(host, port, "/socket.io/?EIO=4");  // Secure WebSocket Connection
    socketIO.onEvent(socketIOEvent);  // Set up event listener
}

void loop() {
    socketIO.loop();  // Keep WebSocket connection alive

    // Reconnect Wi-Fi if disconnected
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️ Wi-Fi Disconnected! Reconnecting...");
        WiFi.reconnect();
        delay(5000);
    }

    delay(10);  // Prevent excessive CPU usage
}

// WebSocket Event Handler
void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case sIOtype_DISCONNECT:
            Serial.println("❌ Disconnected from WebSocket!");
            break;

        case sIOtype_CONNECT:
            Serial.println("✅ Connected to WebSocket!");
            socketIO.send(sIOtype_CONNECT, "/");
            break;

        case sIOtype_EVENT:
            // Convert payload to String
            String event = String((char*)payload);

            // Create a JSON document and deserialize the event payload into it
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, event);

            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }

            // Extract the second element from the array
            if (doc.size() > 1) {
                const char* secondPart = doc[1];
                Serial.println(secondPart);  // Print the second part (LIGHT_OFF or LIGHT_ON)
                
                // Use equals() for string comparison
                if (String(secondPart).equals("LIGHT_ON")) {
                  digitalWrite(LED , HIGH); // Turn on the light 
                } else if (String(secondPart).equals("LIGHT_OFF")) {
                  digitalWrite(LED , LOW);  // Turn off the light
                } else {
                  digitalWrite(LED , LOW);  // Default: Turn off the light
                }
            }
            break;
    }
}




