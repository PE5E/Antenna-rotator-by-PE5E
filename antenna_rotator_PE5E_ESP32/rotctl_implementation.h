#ifndef ROTCTL_IMPLEMENTATION_H
#define ROTCTL_IMPLEMENTATION_H

class Rotctl_impl {
  public:
    Rotctl_impl() = default;             // default constructor
    void set_shared_data(dataset *data); // shared data
    String input_data(String request);   // get data from tcp connection

  private:
    dataset *_data;
};

void Rotctl_impl::set_shared_data(dataset *data) {
  _data = data;
}

 
String Rotctl_impl::input_data(String request) {
  Serial.println("received data in Rotctl_impl::data: ");
  Serial.println(request); 
  String reply = "";

  // no data //
  if(request.length() == 0) {
    // leave reply empty
  }

  // set position //
  if(request.substring(0, 1) == "P" || request.substring(0, 7) == "set_pos") {
    if(request.substring(0, 1) == "P") { 
      request.remove(0, 2); // strip command and space
    }
    if (request.substring(0, 7) == "set_pos") {
      request.remove(0, 8); // strip command and space
    }
    
    // request is now: azimuth space elevation
    request.replace(',','.'); // replace the , in a number with a .
    
    float azimuth = request.substring(0, request.indexOf(" ")).toFloat(); // take first part until a space char and convert to float value
    float elevation = request.substring(request.indexOf(" ") + 1, request.length()).toFloat(); // take the part from the space char until the end and convert to float value
    Serial.print("Azimuth: ");
    Serial.println(azimuth);
    Serial.print("Elevation: ");
    Serial.println(elevation);

    _data->requested_azimuth = azimuth;
    _data->requested_elevation = elevation;

    // for testing purposes:
    _data->current_azimuth = azimuth;
    _data->current_elevation = elevation;
    
    // reply: "RPRT x\n" where x=0 is success and else is error
    reply = "RPRT 0\n";
  }
  
  // get position //
  else if(request.substring(0, 1) == "p" || request.substring(0, 7) == "get_pos") {
    // reply: azimuth\n elevation\n
    // if not available: RPRT x\n where x is the Hamlib error code
    Serial.println("request: get position");
    reply = String(_data->current_azimuth);
    reply.concat('\n');
    reply.concat(_data->current_elevation);
    reply.concat('\n');
  }

  // get information //
  else if(request.substring(0, 1) == "_" || request.substring(0, 8) == "get_info") {
    reply = "Antenna rotator by PE5E\n";
    if(_data->manual_control == true) {
      reply += "Rotator is now in manual mode\n";
    }
  }

  if(_data->manual_control == false) {

    // move rotator //
    if(request.substring(0, 1) == "M" || request.substring(0, 4) == "move") {
      if(request.substring(0, 1) == "M") { 
        request.remove(0, 2); // strip command and space
      }
      if (request.substring(0, 4) == "move") {
        request.remove(0, 5); // strip command and space
      }
      
      // request is now: direction space speed
          
      int req_direction = request.substring(0, request.indexOf(" ")).toInt(); // take first part until a space char and convert to float value
      int req_speed = request.substring(request.indexOf(" ") + 1, request.length()).toInt(); // take the part from the space char until the end and convert to float value

      _data->requested_azimuth = req_direction;
      _data->rotation_speed = req_speed;
      
      reply = "RPRT 0\n";
    }
    
    // stop //
    else if(request.substring(0, 1) == "S" || request.substring(0, 4) == "stop") {
        _data->direction_request = dataset::moving_status::standstill;
      reply = "RPRT 0\n";
    }
  
    // park //
    else if(request.substring(0, 1) == "K" || request.substring(0, 4) == "park") {
        _data->direction_request = dataset::moving_status::standstill;
      reply = "RPRT 0\n";
    }
  
    // reset //
    else if(request.substring(0, 1) == "R" || request.substring(0, 5) == "reset") {
      // reset the rotator
      _data->direction_request = dataset::moving_status::standstill;
      if(request.endsWith("1")) {
        // reset all
      }
      reply = "RPRT 0\n";
    }

  } // if(manual_control == false)

  return reply;
  
}

 
#endif // ROTCTL_IMPLEMENTATION_H
