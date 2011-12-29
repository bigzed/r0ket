#include <sysinit.h>

#include "basic/basic.h"

#include "usetable.h"

void ram(void){
  uint8_t delay = 200;
  uint8_t lr_trigger = 0;
  
  while(true){
    uint8_t cmd = getInputWaitTimeout(delay);
    if(cmd == BTN_UP){
      delay += 1;
    }
    if(cmd == BTN_DOWN){
      delay -= 1;
    }
    
    gpioSetValue (RB_LED0, lr_trigger);
    gpioSetValue (RB_LED2, lr_trigger%2);
    lr_trigger = lr_trigger+1%2;
    
    
    if(cmd == BTN_ENTER){
      gpioSetValue (RB_LED0, 0);
      gpioSetValue (RB_LED2, 0);
      break;
    }
  }
};
