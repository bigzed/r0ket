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

void draw_matrix(){
  font = &Font_Labyrinth;
  lcdFill(0);
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      DoString()
}

static void draw_sprite(char type, char orientation, char x, char y) {
	font = &Font_Labyrinth;
	switch(type) {
		case TYPE_PLAYER:
	}
}




