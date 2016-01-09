#include "botball/botball.h"
#include "botball/botball.hpp"

#include <wallaby/button.h>
#include <wallaby/analog.h>
#include <wallaby/audio.h>
#include <aurora/graphics.h>

#include <iostream>

void colorbar(int i);
void display_clear();

void shut_down_in(double s)
{
  static ShutDownIn *s_instance;
  if(s_instance) {
    std::cout << "Warning: shut_down_in already called once. Ignoring." << std::endl;
    return;
  }
  s_instance = new ShutDownIn(s);
  s_instance->start();
}

// Msecs to pause to let a flash subdue
// Assumption is that if light is detected by consecutive readings FLASHPAUSE apart, 
// it's the start light, not a flash.
// NOTE: when tested with camera, the flash persisted < 0.1 sec
#define FLASHPAUSE 100 

void wait_for_light(int light_port)
{
  static const int THRESHOLD = 60;
  
  // Disable extra buttons
  const bool extraButtons = get_extra_buttons_visible();
  set_extra_buttons_visible(0);
  
  set_a_button_text("VALUE");
  set_c_button_text("-");
  
  bool goodCalib = false;
  while(!goodCalib) {
    std::cout << "CALIBRATION (sensor port " << light_port << ")" << std::endl;
    std::cout << "Press VALUE to see current value" << std::endl;
    
    // ON and OFF calibration
    int calibVals[2] = {-1, -1};
    const std::string calibModes[2] = {"ON", "OFF"};
    for(int i = 0; i < 2; ++i) {
      const std::string bText = std::string("Light is ") + calibModes[i];
      set_b_button_text(bText.c_str());
      std::cout << "Press " << calibModes[i] << " when light is " << calibModes[i] << std::endl;
      while(b_button() == 0) {
        // Print current value
        if(a_button()) {
          const int currVal = analog(light_port);
          std::cout << "Current value: " << currVal << std::endl;
          while(a_button()) {}
        }
        msleep(100);
      }
      calibVals[i] = analog(light_port);
      std::cout << calibModes[i] << " value: " << calibVals[i] << std::endl;
      beep();
      while(b_button()) {}
    }
    
    const int &onVal = calibVals[0];
    const int &offVal = calibVals[1];
    // Good calibration (small value = bright)
    if(offVal - onVal >= THRESHOLD) {
      goodCalib = true;
      std::cout << "Good calibration! (diff = " << (offVal - onVal) << ")" << std::endl;
      std::cout << "WAITING FOR LIGHTS ON" << std::endl;
      graphics_open(240, 25);
      graphics_fill(255, 255, 255);
      graphics_update();
      int cbNum = 0;
      // Loop until light is on
      for(;;) {
        cbNum = (cbNum + 1) % 74;
        colorbar(cbNum);
        msleep(20);
        int currVal = analog(light_port);
        // Reading low enough for light on
        if(currVal - onVal < THRESHOLD) {
          // Do a second check to rule out flash
          msleep(FLASHPAUSE);
          currVal = analog(light_port); 
          if((currVal - onVal) < THRESHOLD) {
            std::cout << "Reading: " << currVal << std::endl;
            std::cout << "*** LIGHTS ARE ON ***" << std::endl;
            break;
          }
        }
      }
      graphics_close();
    }
    // Bad calibration
    else {
      std::cout << "BAD CALIBRATION" << std::endl;
      if(offVal < 256)
        std::cout << "Try adding shielding!" << std::endl;
      else
        std::cout << "Try aiming sensor!" << std::endl;
      msleep(5000);
    }
  }
  
  // Set buttons texts back
  // TODO: Set texts back to their previous value, not defaults
  set_a_button_text("A");
  set_b_button_text("B");
  set_c_button_text("C"); 
  set_extra_buttons_visible(extraButtons);
}

void colorbar(int i) {
  graphics_rectangle_fill(12, 0, 227, 23, 0, 255, 0);
  graphics_circle_fill(11, 11, 12, 0, 255, 0);
  graphics_circle_fill(228, 11, 12, 0, 255, 0);
  if(i < 37) graphics_circle_fill(11 + 6*i, 11, 12, 255, 0, 0);
  else graphics_circle_fill(6 + 6*(74-i), 11, 12, 255, 0, 0);
  graphics_update();
}

// TODO: Find a new home?
void display_clear() {
  printf("\f");
  fflush(stdout);
}