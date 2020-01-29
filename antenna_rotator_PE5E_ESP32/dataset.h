#ifndef DATASET_H
#define DATASET_H

struct dataset {
  float current_azimuth =      180  ;
  float current_elevation =    0    ;
  float requested_azimuth =    180  ;
  float requested_elevation =  0    ;
  int   webserver_address =    80   ;
  int   rotctl_address =       4533 ; 
  int   rotation_speed =       255  ; // 0 = stop, 255 is max
  
  bool  manual_control =       false;
  
  String error_content_html =  ""; // this string will show on HTML page if not empty
  
  enum moving_status {
    standstill,
    cw,  // to the right
    ccw, // to the left
    up,
    down
  };
  moving_status direction_status = standstill;

  const char * moving_status_text[5] = {"standing still", "going clockwise", "going counter clockwise", "going up", "going down"};
};

#endif // DATASET_H
