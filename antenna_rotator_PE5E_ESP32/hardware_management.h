#ifndef HARDWARE_MANAGEMENT_H
#define HARDWARE_MANAGEMENT_H

/*
 * L298N Driver
 *  You can drive a 2-fase stepper motor, a 4-fase stepper motor or 2 DC motors.
 *  Double H bridge drive Chip: L298N (ST NEW)
 *  Driver voltage: 5V - 35V
 *  Driver current: 2A (MAX single bridge)
 *  Max power: 25W
 *  
 * Encoder Library is found here:
 * https://github.com/PaulStoffregen/Encoder
 * 
 * Usage:
 * Set your hardware configuration below 
 *  
 */


#include <Stepper.h> // for using stepper motors
#include <Encoder.h> // for using rotary encoders
#include "dataset.h"

class Hardware_mgmt {
  public:
    Hardware_mgmt();                     // constructor
    ~Hardware_mgmt();                    // destructor
    void set_shared_data(dataset *data); // shared data

    void setup();                        // setup hardware
    void process();                      // check sensors and set motors one time

  private:
    void go_left();
    void go_right();
    void go_up();
    void go_down();
    void stop_azi();
    void stop_ele();

  private:
    dataset *_data = nullptr;
    
  //////////////////////////////////////////////////////////////
  //  USE THIS SECTION TO SELECT YOUR HARDWARE CONFIGURATION  //
  //////////////////////////////////////////////////////////////
  
  // sensors:
    // azimuth potentiometer sensor
    bool   azi_pot_enable =        false;             // enable or disable feature
    int    azi_potentiometer_pin = 33;               // connect the middle pin 
    double azi_pot_min_degree    = 0;                // minimum angle of potentiometer
    int    azi_pot_min_value     = 0;                // minimum analogue reading of potentiometer
    double azi_pot_max_degree    = 340;              // maximum angle of potentiometer
    int    azi_pot_max_value     = 1024;             // maximum analogue reading of potentiometer, 1024 = 3.3V
    
    // elevation potentiometer sensor
    bool   ele_pot_enable =        true;             // enable or disable feature
    int    ele_potentiometer_pin = 34;               // connect the middle pin 
    double ele_pot_min_degree    = -10;              // minimum angle of potentiometer
    int    ele_pot_min_value     = 420;                // minimum analogue reading of potentiometer
    double ele_pot_max_degree    = 75;               // maximum angle of potentiometer
    int    ele_pot_max_value     = 1685;              // maximum analogue reading of potentiometer, 1024 = 3.3V
        
    // azimuth rotary sensor
    bool   azi_rotary_enable =     false;             // enable or disable feature
    int    azi_rotary_pins[2] =    {5, 6};           // set pins here
    int    azi_rotary_pulses =     3600;             // set nr of pulses per full azi rotation (360 degree)

    // elevation rotary sensor
    bool   ele_rotary_enable =     false;             // enable or disable feature
    int    ele_rotary_pins[2] =    {7, 8};           // set pins here
    int    ele_rotary_pulses =     3600;             // set nr of pulses per full ele rotation (360 degree)

  // actuators:
    // azimuth stepper motor
    bool   azi_stepper_enable =    false;             // enable or disable feature
    int    azi_steps_rev =         200;              // set the number of steps per revolution
    int    azi_stepper_pins[4] =   {9, 10, 11, 12};   // set pins here

    // elevation stepper motor
    bool   ele_stepper_enable =    false;              // enable or disable feature
    int    ele_steps_rev =         200;               // number of steps per revolution
    int    ele_stepper_pins[4] =   {13, 14, 15, 16};  // set pins here

    // azimuth DC motor
    bool   azi_dc_enable =         false;             // enable or disable feature
    int    azi_dc_pin =            17;                // pin to drive motor, PWM for speed control
    bool   azi_dc_pwm =            true;              // true for pwm driver, false if only on/off
    int    azi_dc_bridge_pins[2] = {18, 19};          // set pins here

    // elevation DC motor
    bool   ele_dc_enable =         true;              // enable or disable feature
    int    ele_dc_pin =            19;                // pin to drive motor, PWM for speed control
    bool   ele_dc_pwm =            true;              // true for pwm driver, false if only on/off
    int    ele_dc_bridge_pins[2] = {18, 5};           // set pins here

    // Maximum deviation
    double azi_deviation = 2.0;                         // maximum deviation
    double ele_deviation = 2.0;                         // maximum deviation


  //////////////////////////////////////////////////////////////
  //  //  //  //  //  END OF HARDWARE CONFIGURATION   //  //  //
  ////////////////////////////////////////////////////////////// 

    Encoder *azi_encoder = nullptr;
    Encoder *ele_encoder = nullptr;

    Stepper *azi_stepper = nullptr;
    Stepper *ele_stepper = nullptr;
    
    int32_t azi_current_enc = 0; // current reading of azimuth encoder
    int32_t ele_current_enc = 0; // current reading of elevation encoder

    // moving status
    bool azi_moving = false;
    bool ele_moving = false;
};

double doublemap(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Hardware_mgmt::Hardware_mgmt() {
    azi_encoder = nullptr;
    ele_encoder = nullptr;
    azi_stepper = nullptr;
    ele_stepper = nullptr;
}

void Hardware_mgmt::set_shared_data(dataset *data) {
  _data = data;
}

Hardware_mgmt::~Hardware_mgmt() {
  delete azi_encoder;
  delete ele_encoder;
  delete azi_stepper;
  delete ele_stepper;
}

// setup hardware
void Hardware_mgmt::setup() {
  // enable new objects
  if(azi_rotary_enable) {
    azi_encoder = new Encoder(azi_rotary_pins[0], azi_rotary_pins[1]);
  }
  if(ele_rotary_enable) {
    ele_encoder = new Encoder(ele_rotary_pins[0], ele_rotary_pins[1]);
  }
  
  if(azi_stepper_enable) {
    azi_stepper = new Stepper(azi_steps_rev, azi_stepper_pins[0], azi_stepper_pins[1], azi_stepper_pins[2], azi_stepper_pins[3]);
  }
  if(ele_stepper_enable) {
    ele_stepper = new Stepper(ele_steps_rev, ele_stepper_pins[0], ele_stepper_pins[1], ele_stepper_pins[2], ele_stepper_pins[3]);
  }

  // connect sensors
  if(azi_pot_enable) {
    pinMode(azi_potentiometer_pin, INPUT);
    _data->current_azimuth = doublemap(analogRead(azi_potentiometer_pin), azi_pot_min_value, azi_pot_max_value, azi_pot_min_degree, azi_pot_max_degree);
    _data->requested_azimuth = _data->current_azimuth;
  }
  if(ele_pot_enable) {
    pinMode(ele_potentiometer_pin, INPUT);
    _data->current_elevation = doublemap(analogRead(ele_potentiometer_pin), ele_pot_min_value, ele_pot_max_value, ele_pot_min_degree, ele_pot_max_degree);
    _data->requested_elevation = _data->current_elevation;
  }


  if(azi_dc_enable) {
    ledcSetup(0, 20000, 8);                 // channel 0, 20000Hz, 8 bit
    ledcAttachPin(azi_dc_pin, 0);           // attach pin to channel 0
    pinMode(azi_dc_bridge_pins[0], OUTPUT);
    pinMode(azi_dc_bridge_pins[1], OUTPUT);
  }
  if(ele_dc_enable) {
    ledcSetup(1, 20000, 8);                 // channel 1, 20000Hz, 8 bit
    ledcAttachPin(ele_dc_pin, 1);           // attach pin to channel 1
    pinMode(ele_dc_bridge_pins[0], OUTPUT);
    pinMode(ele_dc_bridge_pins[1], OUTPUT);
  }
}

void Hardware_mgmt::go_left() {
  if(azi_dc_enable){
    digitalWrite(azi_dc_bridge_pins[0], LOW);
    digitalWrite(azi_dc_bridge_pins[1], HIGH);
    if(azi_dc_pwm) {
      ledcWrite(0, _data->rotation_speed); // channel 0 is azi motor
    }
    else {
      ledcWrite(0, 255); // channel 0 is ele motor
    }
  }
  else if (azi_stepper_enable) { 
  }
}
void Hardware_mgmt::go_right() {
  if(azi_dc_enable){
    digitalWrite(azi_dc_bridge_pins[0], LOW);
    digitalWrite(azi_dc_bridge_pins[1], HIGH);
    if(azi_dc_pwm) {
      ledcWrite(0, _data->rotation_speed); // channel 0 is azi motor
    }
    else {
      ledcWrite(0, 255); // channel 0 is ele motor
    }
  }
  else if (azi_stepper_enable) { 
  }
}
void Hardware_mgmt::go_up() {
  if(ele_dc_enable){
    digitalWrite(ele_dc_bridge_pins[0], HIGH);
    digitalWrite(ele_dc_bridge_pins[1], LOW);
    if(ele_dc_pwm) {
      ledcWrite(1, _data->rotation_speed); // channel 0 is ele motor
    }
    else {
      ledcWrite(1, 255); // channel 1 is ele motor
    }
  }
  else if (ele_stepper_enable) { 
  }
}
void Hardware_mgmt::go_down() {
  if(ele_dc_enable){
    digitalWrite(ele_dc_bridge_pins[0], LOW);
    digitalWrite(ele_dc_bridge_pins[1], HIGH);
    if(ele_dc_pwm) {
      ledcWrite(1, _data->rotation_speed); // channel 0 is ele motor
    }
    else {
      ledcWrite(1, 255); // channel 1 is ele motor
    }
  }
  else if (ele_stepper_enable) { 
  }
}
void Hardware_mgmt::stop_azi() {
  if(azi_dc_enable){
      ledcWrite(0, 0); // channel 0 is azi motor
      azi_moving = false;
  }
  else if (azi_stepper_enable) { 
  }
}
void Hardware_mgmt::stop_ele() {
  if(ele_dc_enable){
      ledcWrite(1, 0); // channel 1 is ele motor
      ele_moving = false;
  }
  else if (ele_stepper_enable) { 
  }
}

// loop over this process function
void Hardware_mgmt::process() {
  /*
  azi_current_enc = azi_encoder->read();
  ele_current_enc = ele_encoder->read();
  
  azi_stepper->setSpeed(60);        // set speed to 60 rpm
  azi_stepper->step(azi_steps_rev); // rotate 1 rev. positive is CW, negative is CCW
  */

  if(azi_pot_enable) {
    _data->current_azimuth = doublemap(analogRead(azi_potentiometer_pin), azi_pot_min_value, azi_pot_max_value, azi_pot_min_degree, azi_pot_max_degree);
  }
  if(ele_pot_enable) {
    _data->current_elevation = doublemap(analogRead(ele_potentiometer_pin), ele_pot_min_value, ele_pot_max_value, ele_pot_min_degree, ele_pot_max_degree);
  }
  


  switch(_data->direction_request) {
    case dataset::moving_status::ccw:
      if(ele_moving) {
        stop_ele();
      }
      _data->requested_azimuth -= azi_deviation + 1;
      go_left();
      _data->direction_status = dataset::moving_status::ccw;
      break;
  
    case dataset::moving_status::cw:
      if(ele_moving) {
        stop_ele();
      }
      _data->requested_azimuth += azi_deviation + 1;
      go_right();
      _data->direction_status = dataset::moving_status::cw;
      break;
  
    case dataset::moving_status::up:
      if(azi_moving) {
        stop_azi();
      }
      _data->requested_elevation += ele_deviation + 1;
      go_up();
      _data->direction_status = dataset::moving_status::up;
      break;
  
    case dataset::moving_status::down:
      if(azi_moving) {
        stop_azi();
      }
      _data->requested_elevation -= ele_deviation + 1;
      go_down();
      _data->direction_status = dataset::moving_status::down;
      break;
  
    case dataset::moving_status::standstill:
      stop_azi();
      stop_ele();
      _data->requested_azimuth = _data->current_azimuth;
      _data->requested_elevation = _data->current_elevation;
      break;

    case dataset::moving_status::none:
      break;
  }

  


  if(_data->requested_azimuth < (_data->current_azimuth - azi_deviation)) {
    go_left();
    azi_moving = true;
    _data->direction_status = dataset::moving_status::ccw; 
    _data->direction_request = dataset::moving_status::none; 
  }
  else if(_data->requested_azimuth > (_data->current_azimuth + azi_deviation)) {
    go_right();
    azi_moving = true;
    _data->direction_status = dataset::moving_status::cw; 
    _data->direction_request = dataset::moving_status::none; 
    //////////////////

    // fix something here

    ///////////
  }
  else {
    stop_azi();
    if(!ele_moving){
      _data->direction_status = dataset::moving_status::standstill;
    }
  }
  
  if(_data->requested_elevation < (_data->current_elevation - ele_deviation)) {
    go_down();
    ele_moving = true;
    _data->direction_status = dataset::moving_status::down; 
    _data->direction_request = dataset::moving_status::none; 
  }
  else if(_data->requested_elevation > (_data->current_elevation + ele_deviation)) {
    go_up();
    ele_moving = true;
    _data->direction_status = dataset::moving_status::up; 
    _data->direction_request = dataset::moving_status::none; 
  }
  else {
    stop_ele();
    if(!azi_moving){
      _data->direction_status = dataset::moving_status::standstill;
    }
  }  
  
}





#endif // HARDWARE_MANAGEMENT_H
