

/* Antenna rotator by PE5E
 *  
 * 
 *  
 *  
 *  
 *  
 *  
 *  
 */

#include <ESPAsyncWebServer.h>
#include "index.h" //HTML webpage contents
// #include <ESPAsyncTCP.h>
 
const char* ssid = "Ziggo";
const char* password = "Tijdelijk_Netwerk_Voor_Gasten#";
 
int ledPin = 2; // built in led
int current_heading = 0; 

enum moving_status {
  standstill,
  cw,  // to the right
  ccw, // to the left
  up,
  down
};
const char * moving_status_text[] = {"standing still", "clockwise (to the right)", "counter clockwise (to the left)", "UP", "DOWN"};

moving_status direction_status = standstill;

AsyncWebServer http_server(80);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

// AsyncServer control_port(4453);

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  Serial.println("Received unknown request.");
  request->send(404);
}

// make HTML pages dynamic. replace variables like %variable% with server data
String processor(const String& var)
{
  if(var == "DIRECTION_STATUS"){
    return moving_status_text[direction_status];
  }

  if(var == "CURRENT_HEADING"){
    char buffer[10];
    itoa(current_heading, buffer, 10);
    return buffer;
  }
  
  return String();
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
  Serial.println("Websocket onEvent"); 
}

void processRequest(AsyncWebServerRequest *request){
    if(request->hasParam("direction")) {
      AsyncWebParameter* p = request->getParam("direction");
      String directionCommand = p->value().c_str();
      if(directionCommand == "UP") {
        direction_status = up;
      }
      else if(directionCommand == "DOWN") {
        direction_status = down;
      }
      else if(directionCommand == "CCW->LEFT") {
        direction_status = ccw;
      }
      else if(directionCommand == "CW->RIGHT") {
        direction_status = cw;
      }
      else if(directionCommand == "STOP") {
        direction_status = standstill;
      }
    }

  //List all parameters
//  int params = request->params();
//  for(int i=0;i<params;i++){
//    AsyncWebParameter* p = request->getParam(i);
//    if(p->isFile()){ //p->isPost() is also true
//      Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//    } else if(p->isPost()){
//      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//    } else {
//      Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//    }
//  }  
}

void setup() {
  Serial.begin(115200);
  delay(10);

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

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // HIGH is off
 
// Connect to WiFi network
  Serial.println();
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
 
  // Start the server
  http_server.begin();
//  control_port.begin();
  
  Serial.println("http_server started");
// Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  if(direction_status > 0) {
    digitalWrite(ledPin, LOW); // LOW is on
  }
  else {
    digitalWrite(ledPin, HIGH); // HIGH is off
  }

  if(direction_status == cw) {
    current_heading = (current_heading + 1) % 360;
  }
  else if(direction_status == ccw) {
    current_heading -= 1;
    if(current_heading == -1) {
      current_heading = 359;
    }
  }
  delay(100); // to prevent overheating with async ;-)
}
