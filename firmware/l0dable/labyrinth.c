#include <sysinit.h>
#include <string.h>

#include "basic/random.h"
#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "lcd/fonts.h"
#include "lcd/fonts/labyrinth.h"

#include "usetable.h"
/*@author Steve Dierker <steve.dierker@obstkiste.org>
 */

#define BLOCK_SIZE 4
#define TYPE_PLAYER 1
#define NORMAL 0
#define LTop 1
#define RTop 2
#define LBot 3
#define RBot 4
#define LLef 5
#define RLef 6
#define LRig 7
#define RRig 8

struct playerstate {
  char moving_state;
  char posX, posY;
} player;

void ram(void) {
  init();
  do {
    lcdFill(0);
    //------------
    move_player();
    draw_player();
    //------------
    lcdDisplay();
    delayms(12);
  }while(gpioGetValue(RB_BTN4)!=0) 
}


// initialize all the things
void init() {
  init_player();
}

void init_player() {
  player.moving_state = 0;
  // Grid is 24x17 blocks
  player.posX = 12;
  player.posY = 17;
}

// move all the things
void move_player() {
  if(gpioGetValue(RB_BTN0)==0 && player.posX>0)
    player.posX-=1;
  if(gpioGetValue(RB_BTN1)==0 && player.posX<12)
    player.posX+=1;
  if(gpioGetValue(RB_BTN2)==0 && player.posY>0)
    player.posY-=1;
  if(gpioGetValue(RB_BTN3)==0 && player.posY<17)
    player.posY+=1;
}

// draw all the things
void draw_player() {
  draw_sprite(TYPE_PLAYER, player.posX*BLOCK_SIZE, player.posY*BLOCK_SIZE);
}

void draw_orientated_player(char x, char y) {
  switch(player.moving_state) {
    case NORMAl:
      DoChar(x,y-1,'P');
      break;
    case LTop:
      DoChar(x,y-1,'Q');
      break;
    case RTop:
      DoChar(x,y-1,'R');
      break;
    case LBot:
      DoChar(x,y-1,'S');
      break;
    case RBot:
      DoChar(x,y-1,'T');
    case LLef:
      DoChar(x,y-1,'U');
      break;
    case RLef:
      DoChar(x,y-1,'V');
      break;
    case LRig:
      DoChar(x,y-1,'W');
      break;
    case RRig:
      DoChar(x,y-1,'V');
  }
}

void draw_sprite(char type, char x, char y) {
  // TODO: set font!
  switch(type) {
    case TYPE_PLAYER:
      draw_orientated_player(x,y)
      }
      break;
  }
}
