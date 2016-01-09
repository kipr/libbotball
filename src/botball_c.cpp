#include "botball/botball.h"
#include "botball/botball.hpp"

#include <wallaby/button.h>
#include <wallaby/analog.h>
#include <wallaby/audio.h>
#include <aurora/graphics.h>

#include <iostream>

/*#include "kovan/button.h"
#include "kovan/util.h"
#include "kovan/display.h"
#include "kovan/analog.h"
#include "kovan/audio.h"
#include "kovan/graphics.h"
#include "kovan/thread.hpp"
#include "kovan/general.h"
#include "kovan/create.hpp"
#include "kovan/compat.hpp"

#include <cstdlib>
#include <iostream>*/

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

// Reworked for flash suppression January 6, 2015
// Graphics signal for waiting
#define FLASHPAUSE 100  // msecs to pause to let a flash subside
/* Assumption is that if light is detected by consecutive readings FLASHPAUSE apart, 
   it's the start light, not a flash
   NOTE: when tested with camera, the flash persisted < 0.1 sec
*/
#define THRESHOLD 60    // minimum discrimination between light and dark
#define RED 255,0,0
#define GREEN 0,255,0
void wait_for_light(int light_port)
{
  int xBut, OK=0, onval, offval, reading, cb=0, i; 
  xBut=get_extra_buttons_visible(); // in case user has extra buttons visible
  set_extra_buttons_visible(0);     //    turn them off
  set_a_button_text("-");
  set_c_button_text("-");
  while (!OK) {
    set_b_button_text("Light is ON");
    display_clear();
    printf("CALIBRATE: sensor port %d\n", light_port);
    printf("press ON when light is on\n");
    while(b_button()==0){
      msleep(100);
      onval=analog(light_port); // sensor value when light is on
      printf("light on value is = %d\n", onval);
    }
    set_b_button_text("Light is OFF");
    printf("light on value is = %d\n", onval);
    msleep(200);
    beep();
    while (b_button()); // debounce B button
    printf("press OFF when light is off\n");
    while (b_button()==0) {
      offval=analog(light_port);
      printf("light off value is %d\n", offval);
      msleep(100);
    }
    offval=analog(light_port); // sensor value when light is off
    printf("light off value is = %d\n", offval);
    msleep(200);
    if ((offval-onval)>=THRESHOLD) { // bright = small values
      OK=1;
      printf("Good calibration!\n");
      printf("Diff = %d: WAITING FOR LIGHTS ON\n", offval-onval);
      graphics_open(240,25);
      graphics_fill(255,255,255);
      graphics_update();
      while (1) { // loop until light verified on
        colorbar((i++)%74);
        msleep(20);
        reading=analog(light_port);
        printf("Current reading: %d\n", reading);
        if ((reading-onval) < THRESHOLD) { // reading is low enough for light on
          msleep(FLASHPAUSE); // pause
          reading=analog(light_port);  // get second reading to rule out flash  
           if ((reading-onval) < THRESHOLD) break; // if still low enough, light is on
        }
      }
      graphics_close();
      printf("Reading: %4d *** LIGHTS ARE ON ***\n", reading);
    }
    else {
      printf("BAD CALIBRATION\n");
      if (offval<256) {
        printf("Add shielding!!\n");
        msleep(5000);
      }
      else {
        printf("Aim sensor!!\n");
        msleep(5000);
      }
    }
  }
  set_a_button_text("A"); set_b_button_text("B"); set_c_button_text("C"); 
  set_extra_buttons_visible(xBut); // in case user had extra buttons visible
}

void colorbar(int i) {
  graphics_rectangle_fill(12,0,227,23,GREEN);
  graphics_circle_fill(11,11,12,GREEN);
  graphics_circle_fill(228,11,12,GREEN);
  if (i<37) graphics_circle_fill(11+6*i,11,12,255,0,0);
  else graphics_circle_fill(6+6*(74-i),11,12,255,0,0);
  graphics_update();
}

// TODO: Find a new home?
void display_clear() {
  printf("\f");
  fflush(stdout);
}