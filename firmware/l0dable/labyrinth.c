#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "lcd/fonts.h"

#include "lcd/fonts/invaders.h"

#include "usetable.h"

/*@author Steve Dierker <steve.dierker@obstkiste.org>, Jannis Ihrig <jannis.ihrig@fu-berlin.de>
 */
 
/**************************************************************************/
#define BLOCK_SIZE 4
#define ORI_UP 0
#define ORI_DOWN 1
#define ORI_LEFT 2
#define ORI_RIGHT 3
#define TYPE_FREE 0
#define TYPE_PLAYER 1
#define TYPE_WALL 2
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

bool matrix[24][17];

void ram(void) {
  while(true) {
    /* initialize */
	  init();
	  /* intro */
    if (!screen_intro())
      return;
      /* game */
      do {
        lcdFill(0);
        //------------
        move_player();
        draw_player();
        //------------
        lcdDisplay();
        delayms(12);
      }while(gpioGetValue(RB_BTN4)!=0) 
    /* exit */
		if (!screen_gameover())
			return;
	}
}

/* initialize all the things */
void init() {
  init_player();
}

void init_player() {
  player.moving_state = 0;
  // Grid is 24x17 blocks
  player.posX = 12;
  player.posY = 17;
}

/* different screens */
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
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      matrix[i][j] = false;
    }
  }
  bool free = true;
  char x = 0;
  char y = 5;
  char org = 4; //no origin
  char direct = 4;
  for(i=0; i<5; i++){
    while(free){
      while((direct==org){
        direct = getRandom%4;
      }
      if(direct==0) y = y-1%17;
      if(direct==1) x = x+1%24;
      if(direct==2) y = y+1%17;
      if(direct==3) y = y+1%24;
      if(matrix[x][y] == false){
        matrix[x][y] = true;
      }
      else{
        break;
      }
    }
  }
}

/* actual game code */
// move player
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

/*draw all things */

void draw_sprite(char type, char x, char y) {
  // TODO: set font!
  switch(type) {
    case TYPE_PLAYER:
      draw_orientated_player(x,y)
      }
      break;
  }
}

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

void draw_matrix(){
  font = &Font_Labyrinth;
  lcdFill(0);
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      DoString()
}
