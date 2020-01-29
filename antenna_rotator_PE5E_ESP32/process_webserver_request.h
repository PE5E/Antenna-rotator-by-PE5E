#ifndef PROCESS_WEBSERVER_REQUEST
#define PROCESS_WEBSERVER_REQUEST

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  Serial.println("Received unknown request.");
  request->send(404);
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
  Serial.println("Websocket onEvent"); 
}

void processRequest(AsyncWebServerRequest *request){
  shared_data->error_content_html = ""; // remove last error
  
  if(shared_data->manual_control == true) {
    if(request->hasParam("direction")) {
      AsyncWebParameter* p = request->getParam("direction");
      String directionCommand = p->value().c_str();
      if(directionCommand == "UP") {
        shared_data->direction_request = dataset::moving_status::up;
      }
      else if(directionCommand == "DOWN") {
        shared_data->direction_request = dataset::moving_status::down;
      }
      else if(directionCommand == "CCW->LEFT") {
        shared_data->direction_request = dataset::moving_status::ccw;
      }
      else if(directionCommand == "CW->RIGHT") {
        shared_data->direction_request = dataset::moving_status::cw;
      }
      else if(directionCommand == "STOP") {
        shared_data->direction_request = dataset::moving_status::standstill;
      }
    }

    if(request->hasParam("azi")) {
      AsyncWebParameter* p = request->getParam("azi");
      String azi = p->value().c_str();
      shared_data->requested_azimuth = azi.toFloat();
    }
    if(request->hasParam("ele")) {
      AsyncWebParameter* p = request->getParam("ele");
      String ele = p->value().c_str();
      shared_data->requested_elevation = ele.toFloat();
      if(shared_data->requested_elevation > 90.0) {
        shared_data->error_content_html = "Elevation is more than the maximum value. Please select a lower number.";
        Serial.println("Error. Elevation above max");
      }
    }

    if(request->hasParam("set_speed")) {
      AsyncWebParameter* p = request->getParam("set_speed");
      String ele = p->value().c_str();
      shared_data->rotation_speed = ele.toInt();
    }
  }

    if(request->hasParam("controlmode")) {
      AsyncWebParameter* p = request->getParam("controlmode");
      String control_mode = p->value().c_str();
      if(control_mode == "MANUAL") {
        shared_data->manual_control = true;
      }
      else if(control_mode == "AUTOMATIC") {
        shared_data->manual_control = false;
      }
    }
}

#endif
