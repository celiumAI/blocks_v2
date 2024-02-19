#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define SSID "WIFI_SSID"
#define PASS "WIFI_PASS"

#define HEARTBEAT_URL "NETWORK_HEARTBEAT_URL"

ESP8266WebServer webserver(80);

unsigned long last_heartbeat = 0;
unsigned long heartbeat_interval = 1000;

void beatHeart() {
    if (millis() < last_heartbeat) {
        last_heartbeat = millis();
    }
    if (millis() - last_heartbeat > heartbeat_interval) {
        Serial.println("pulse!");
        last_heartbeat = millis();
        HTTPClient http;
        WiFiClient client;
        http.begin(client, HEARTBEAT_URL);
        int httpCode = http.GET();
        http.end();
    }
}

void endpointHome() { 
  webserver.send(200, "text/html", "NODE_HOME"); 
}

void endpointNode() { 
  webserver.send(200, "application/json", "NODE_JSON"); 
}

// Handle 404
void notFound(){ 
  webserver.send(404, "text/plain", "404: Not found"); 
}

void setup()
{
  // Setup serial port
  Serial.begin(115200);
  Serial.println();

  //Begin WiFi
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(100); }

  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  webserver.on("/", endpointHome);
  webserver.on("/node", endpointNode);
  webserver.onNotFound(notFound);
  webserver.begin();
}

// Listen for HTTP requests
void loop(void){ 
  webserver.handleClient(); 
  beatHeart();
}
