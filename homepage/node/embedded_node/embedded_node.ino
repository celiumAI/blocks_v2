#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define SSID "testbench"
#define PASS "testbench"

#define HEARTBEAT_URL "http://192.168.237.246:5000/heartbeat"

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
  webserver.send(200, "text/html", "<!DOCTYPE html>\n<html>\n  <head>\n    <title>homepage</title>\n  </head>\n  <body>\n    <p>node id: <span id='node-id'></span></p>\n    <p>node name: <span id='node-name'></span></p>\n    <p>node timestamp: <span id='node-timestamp'></span></p>\n    <p>node origin: <span id='node-origin'></span></p>\n    <script>\n        let node = {\n            id: 'bfc45d03-9065-4303-807f-e1cec5113817',\n            name: 'homepage_demo',\n            timestamp: '1708369543545',\n            origin: 'blocks_v2 homepage demo',\n        }\n        console.log(node)\n        for (let key in node) {\n            if (node.hasOwnProperty(key)) {\n                document.getElementById(`node-${key}`).innerText = node[key]\n            }\n        }\n    </script>\n  </body>\n</html>"); 
}

void endpointNode() { 
  webserver.send(200, "application/json", "{\n    \"NODE_ID\": \"bfc45d03-9065-4303-807f-e1cec5113817\",\n    \"NODE_NAME\": \"homepage_demo\",\n    \"NODE_TIMESTAMP\": \"1708369543545\",\n    \"NODE_ORIGIN\": \"blocks_v2 homepage demo\"\n}"); 
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
