/* Antenna rotator by PE5E
 * 
 * This project is based upon the ESP32 development board.
 * The rotator motors and sensors can be connected to the ESP32 board, eventually through a bridge.
 * There is a webserver at port 80 to show the user the status of the rotator and offer manual control.
 * There is also a service listening on port 4533 for rotctl commands, so you can control the rotator with a pc.
 * 
 *  
 *  
 *  
 */

#include "Arduino.h"

#include "dataset.h"
dataset   *shared_data = new dataset;           // holds all shared data

#include "ESPAsyncWebServer.h"
AsyncWebServer http_server(shared_data->webserver_address);

#include "rotctl_implementation.h"     // implementation of the rotctl protocol
Rotctl_impl rotctl;                    // translates rotctldata to actions

#include "controlport_server.h"        // controlport server
#include "hardware_management.h"       // controlling motors and reading sensors
#include "AsyncTCP.h"                  // used by rotctl
#include "html_processor.h"            // makes the HTML page dynamic
#include "process_webserver_request.h" // processes the server requests


#include "index.h"                     // HTML webpage contents
 
const char* ssid = "Ziggo";
const char* password = "Tijdelijk_Netwerk_Voor_Gasten#";
 
int       ledPin = 2    ; // built in led

AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

Hardware_mgmt hardware;



void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // HIGH is off
  Serial.begin(115200);
  delay(10);

  rotctl.set_shared_data(shared_data);
  hardware.set_shared_data(shared_data);

  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  http_server.addHandler(&ws);

  // attach AsyncEventSource
  http_server.addHandler(&events);

  http_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    // request->send(SPIFFS, "/index.htm", String(), false, processor);
  });
  
  http_server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request){
    processRequest(request); // do something with the request
    request->send_P(200, "text/html", index_html, processor);
  });

  // rotctl:
  AsyncServer* control_server = new AsyncServer(shared_data->rotctl_address); // server for rotctl commands
  control_server->onClient(&handleNewClient, control_server);
 
// Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the servers
  http_server.begin();
  control_server->begin();
  
  Serial.println("http server and control server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  hardware.setup();
}
 
void loop() {
  if(shared_data->direction_status > 0) {
    digitalWrite(ledPin, LOW); // LOW is on
  }
  else {
    digitalWrite(ledPin, HIGH); // HIGH is off
  }

  hardware.process();
  delay(1000); 
}
