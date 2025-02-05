#include <SocketIOclient.h>   // Socket.IO client library
#include <ESP8266WiFi.h>      // ESP8266 Wi-Fi library
#include <ArduinoJson.h>      // ArduinoJson library

// Wi-Fi Credentials
const char* ssid = "Laptop";
const char* password = "8918172922";

// WebSocket Server Details (Change IP if using local server)
const char* host = "192.168.238.231";  // Replace with your server IP
const int port = 3000;

SocketIOclient socketIO;  // WebSocket client object

void setup() {
    Serial.begin(9600);  // Start Serial Monitor
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);  // Turn off the light

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

    // Connect to WebSocket Server
    socketIO.begin(host, port, "/socket.io/?EIO=4");  // EIO=4 for Socket.IO v4+
    socketIO.onEvent(socketIOEvent);  // Set up the event listener for all events
}

void loop() {
    socketIO.loop();  // Keep WebSocket connection alive

    // Reconnect Wi-Fi if disconnected
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("⚠️ Wi-Fi Disconnected! Reconnecting...");
        WiFi.begin(ssid, password);  // Restart Wi-Fi connection
        delay(5000);
    }
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
                  digitalWrite(D4, LOW); // Turn on the light
                } else if (String(secondPart).equals("LIGHT_OFF")) {
                  digitalWrite(D4, HIGH);  // Turn off the light
                }else{
                  digitalWrite(D4, HIGH);  // Turn off the light
                  }
            }
            break;
    }
}








