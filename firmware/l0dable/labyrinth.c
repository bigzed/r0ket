#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "lcd/fonts.h"
#include "lcd/fonts/invaders.h"

#include "funk/mesh.h"

#include "usetable.h"
/**************************************************************************/

#define ORG_NONE 0
#define ORG_UP 1
#define ORG_DOWN 2
#define ORG_LEFT 3
#define ORG_RIGHT 4

bool matrix[24][17];

void ram(void) {
	while(true) {
		if (!screen_intro())
			return;
      lcdPrintln("game");
      
		if (!screen_gameover())
			return;
	}
}

bool screen_intro() {
  char key = BTN_NONE;
  while(key == BTN_NONE){
    lcdClear();
    lcdPrintln("Labyrinth");
    lcdRefresh();
    getInputWaitTimeout(5000)
  }
  return !(key == BTN_LEFT);
}

bool screen_gameover() {
	char key = BTN_NONE;
	while(key == BTN_NONE) {
    lcdClear();
    lcdPrintln("Game Over");
    lcdRefresh();
		key=getInputWaitTimeout(5000);
	}
	return !(key==BTN_LEFT);
}

void generate_matrix(bool[24][17]s matrix){
  bool free = true;
  bool org = ORG_NONE;
  uint16_t x = getRandom()%24;
  uint16_t y = get
  for(i=0; i<5; i++){
    while()
    getRandom()%24;
}




