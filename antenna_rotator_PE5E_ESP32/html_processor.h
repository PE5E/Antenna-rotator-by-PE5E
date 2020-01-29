#ifndef HTML_PROCESSOR
#define HTML_PROCESSOR

// make HTML pages dynamic. replace variables like %variable% with server data
String processor(const String& var)
{
  if(var == "DIRECTION_STATUS"){
    return shared_data->moving_status_text[shared_data->direction_status];
  }

  if(var == "CURRENT_AZIMUTH"){
    return String(shared_data->current_azimuth);
  }

  if(var == "CURRENT_ELEVATION"){
    return String(shared_data->current_elevation);
  }

  if(var == "REQUESTED_AZIMUTH"){
    return String(shared_data->requested_azimuth);
  }

  if(var == "REQUESTED_ELEVATION"){
    return String(shared_data->requested_elevation);
  }
  
  if(var == "ROTATION_SPEED"){
    return String(shared_data->rotation_speed);
  }
  
  if(var == "MANUAL_CONTROL_TEXT"){
      if(shared_data->manual_control) {
        return String("manually");
      }
      else {
        return String("automatically");
      }
  }

  if(var == "CURRENT_ACTION_DEG"){
      switch(shared_data->direction_status) {
        case shared_data->moving_status::standstill:
          return String("0");
          break;
        case shared_data->moving_status::cw:
          return String("90");
          break;
        case shared_data->moving_status::ccw:
          return String("270");
          break;
        case shared_data->moving_status::up:
          return String("0");
          break;
        case shared_data->moving_status::down:
          return String("180");
          break;
      }
  }

  if(var == "ERROR_CODE_HTML") {
    if(shared_data->error_content_html != "") {
      String html = R"12345abcd(<div style="padding:10px;border:1px solid;border-radius:5px;color:#ff0000"><H2 style="color:#ff0000">Error</H2><b style="color:#ff0000">)12345abcd";
      html += shared_data->error_content_html;
      html += "</b></div>"; 
      return html;
    }
  }
  
    
  return String();
}

#endif
