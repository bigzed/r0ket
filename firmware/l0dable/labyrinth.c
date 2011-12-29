#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"
#include "basic/random.h"

#include "lcd/render.h"
#include "lcd/display.h"

#include "lcd/fonts.h"
#include "lcd/fonts/labyrinth.h"

#include "usetable.h"

/*@author Steve Dierker <steve.dierker@obstkiste.org>, Jannis Ihrig <jannis.ihrig@fu-berlin.de>
 */
 
/**************************************************************************/
#define BLOCK_SIZE 4

//sprite types
#define TYPE_FLOOR 0
#define TYPE_PLAYER 1
#define TYPE_WALL 2

//directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define NO_ORIENTATION 4

//player arms
#define ARM_LEFT 0
#define ARM_RIGHT 1
#define ARM_NONE 2

//player start position
#define START_X 1
#define START_Y 1

//matrix definitions
#define WAYS_AMOUNT 4

//player orientations
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
  char orientation;
  char arm_state;
  char posX, posY;
  char player_char;
} player;

char matrix[24][17];

void ram(void) {
  while(true) {
    /* initialize */
	  init();
	  /* intro */
    if (!screen_intro())
      return;
      /* game */
      matrix = generate_matrix;
      do {
        lcdFill(0);
        //------------
        move_player();
        draw_matrix();
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
  player.arm_state = ARM_NONE;
  // Grid is 24x17 blocks
  player.posX = START_X;
  player.posY = START_Y;
  player.player_char = 'P';
}

/* different screens */
bool screen_intro() {
  char key = BTN_NONE;
  while(key == BTN_NONE){
    lcdFill(0);
    DoString(16,40,"LABYRINTH");
    lcdDisplay();
    getInputWaitTimeout(5000)
  }
  return !(key == BTN_LEFT);
}

bool screen_gameover() {
	char key = BTN_NONE;
	while(key == BTN_NONE) {
    lcdFill(0);
		DoString (14,32, "GAME OVER");
		lcdDisplay();
		key=getInputWaitTimeout(5000);
	}
	return !(key==BTN_LEFT);
}

/* actual game code */

void generate_matrix(char[24][17] matrix){
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      matrix[i][j] = 'w';
    }
  }
  bool free = true;
  char x = 1;
  char y = 1;
  char org = 4; //no origin
  char direct = 4;
  for(i=0; i<=WAYS_AMOUNT; i++){
    while(free){
      while(direct==org){
        direct = getRandom%4;
      }
      if(direct==0) y = y-1%17;
      if(direct==1) x = x+1%24;
      if(direct==2) y = y+1%17;
      if(direct==3) y = y+1%24;
      if(matrix[x][y] == false){
        matrix[x][y] = 'f';
      }
      else{
        break;
      }
    }
    x = getRandom()%24;
    y = getRandom()%17;
  }
}

void move_player() {
  char orientation = NO_ORIENTATION;
  char newX = player.posX;
  char newY = player.posY;
  if(gpioGetValue(RB_BTN0)==0 && player.posX>0)
    newX-=1;
    player.orientation = LEFT;
  if(gpioGetValue(RB_BTN1)==0 && player.posX<12)
    newX+=1;
    player.orientation = RIGHT;
  if(gpioGetValue(RB_BTN2)==0 && player.posY>0)
    newY-=1;
    player.orientation = UP;
  if(gpioGetValue(RB_BTN3)==0 && player.posY<17)
    newY+=1;
    player.orientation = DOWN;
  
  if(matrix[newX][newY] == 'f'){
    player.posX = newX;
    player.posY = newY;
    player.arm_state = player.arm_state+1%2;
  }
  
  orientate_player();
  
  matrix[player.posX][player.posY] = player.player_char;
}

void orientate_player(){
  switch(player.orientation) {
    case NO_ORIENTATION:
      player_char = 'P';
      break;
    case LEFT:
      player.player_char = player.arm_state?'V':'U';
      break;
    case RIGHT:
      player.player_char = player.arm_state?'X':'W';
      break;
    case UP:
      player.player_char = player.arm_state?'R':'Q';
      break;
    case DOWN:
      player.player_char = player.arm_state?'T':'S';
      break;
  }
}




/*draw all things */

void draw_matrix(){
  font = &Font_Labyrinth;
  lcdFill(0);
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      DoChar(matrix[i][j], i*BLOCK_SIZE, j*BLOCK_SIZE);
    }
  }
}
