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

//coords
//#define ZERO_X -8;
//#define ZERO_Y -5;

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
#define START_X 10
#define START_Y 5

//matrix definitions
#define WAYS_AMOUNT 4

/* sprites */
//player orientations, direction + left/right arm
#define CENTRAL 'P'
#define UPLA 'Q'
#define UPRA 'R'
#define DOWNLA 'S'
#define DOWNRA 'T'
#define LEFTLA 'U'
#define LEFTRA 'V'
#define RIGHTLA 'W'
#define RIGHTRA 'X'

void init();
void init_player();
bool screen_intro();
bool screen_gameover();
void generate_matrix();
void move_player();
void orientate_player();
void update_matrix(char type, uint32_t, uint32_t);
void draw_matrix();

struct playerstate {
  char orientation;
  char arm_state;
  uint8_t posX, posY;
  char player_char;
} player;

char matrix[24][17];

void ram(void) {
  while(true) {
    
	  /* intro */
    if (!screen_intro())
      return;
    /* initialize */
    init();
    /* game */
    do {
      //------------
      move_player();
      lcdFill(0);
      draw_matrix();
      delayms(100);
      //------------
    }while(getInputRaw() != BTN_ENTER); 
    /* exit */
		if (!screen_gameover())
			return;
	}
}

/* initialize all the things */
void init() {
  generate_matrix();
  init_player();
  draw_matrix();
  delayms(500);
}

void init_player() {
  player.arm_state = ARM_NONE;
  // Grid is 24x17 blocks
  player.posX = START_X;
  player.posY = START_Y;
  player.player_char = 'P';
  update_matrix('P', START_X, START_Y);
}

/* different screens */
bool screen_intro() {
  lcdFill(0);
  font = &Font_7x8;
  DoString(16,40,"LABYRINTH");
  lcdDisplay();
  delayms(500);
  char key;
  while(key = getInputWaitTimeout(5000))
  if(key == BTN_LEFT){
    delayms(500);
    return 0;
	}
	else if(key == BTN_ENTER) {
    return 1;
    delayms(500);
	}
	return 1;
}

bool screen_gameover() {
  lcdFill(0);
  font = &Font_7x8;
	DoString (14,24, "GAME OVER");
	DoString (10,32, "enter: again");
	DoString (14,40, "left: exit");
	lcdDisplay();
	delayms(500);
	if(getInputWaitTimeout(5000) == BTN_LEFT){
	  return 0;
	}
	else {
    return 1;
	}
}

/* actual game code */

void generate_matrix(){
  uint8_t i,j;
  for(i=0; i<24; i++){
    for(j=0; j<17; j++){
      matrix[i][j] = 'f';
    }
  }
/*  bool free = true;*/
/*  uint32_t x = 1;*/
/*  uint32_t y = 1;*/
/*  char org = 4; //no origin*/
/*  char direct = 4;*/
/*  for(i=0; i<=WAYS_AMOUNT; i++){*/
/*    while(free){*/
/*      while(direct==org){*/
/*        direct = getRandom()%4;*/
/*      }*/
/*      if(direct==0) y = y-1%17;*/
/*      if(direct==1) x = x+1%24;*/
/*      if(direct==2) y = y+1%17;*/
/*      if(direct==3) y = y+1%24;*/
/*      if(matrix[x][y] == 'w'){*/
/*        matrix[x][y] = 'f';*/
/*      }*/
/*      else{*/
/*        break;*/
/*      }*/
/*    }*/
/*    x = getRandom()%24;*/
/*    y = getRandom()%17;*/
/*  }*/
}

void move_player() {
  uint32_t newX = player.posX;
  uint32_t newY = player.posY;
  char key = getInputWaitTimeout(20);
  if(key == BTN_LEFT && player.posX>0){
    newX-=1;
    player.orientation = LEFT;
  }
  if(key == BTN_RIGHT && player.posX<24){
    newX+=1;
    player.orientation = RIGHT;
  }
  if(key == BTN_UP && player.posY>0){
    newY-=1;
    player.orientation = UP;
  }
  if(key == BTN_DOWN && player.posY<17){
    newY+=1;
    player.orientation = DOWN;
  }
  
  if(matrix[newX][newY] == 'f'){
    update_matrix(player.posX,player.posY,'f');
    player.posX = newX;
    player.posY = newY;
    player.arm_state = player.arm_state+1%2;
  }
  update_matrix(player.player_char, player.posX, player.posY);
  orientate_player();
  
}

void orientate_player(){
  switch(player.orientation) {
    case NO_ORIENTATION:
      player.player_char = 'P';
      break;
    case LEFT:
      player.player_char = (player.arm_state == 0)? LEFTLA:LEFTRA;
      break;
    case RIGHT:

      player.player_char = (player.arm_state == 0)? RIGHTLA:RIGHTRA;
      break;
    case UP:
      player.player_char = (player.arm_state == 0)? UPLA:UPRA;
      break;
    case DOWN:
      player.player_char = (player.arm_state == 0)? DOWNLA:DOWNRA;
      break;
  }
}

void update_matrix(char type, uint32_t x, uint32_t y){
  matrix[x][y] = type;
}


/*draw all things */

void draw_matrix(){
//  uint8_t i,j;
  font = &Font_Labyrinth;
  lcdFill(0);
// for(i=0; i<24; i++){
//    for(j=0; j<17; j++){
//      DoChar(i*BLOCK_SIZE, j*BLOCK_SIZE, matrix[i][j    DoChar(j*4,0, 'P');
//]);
//    }
//  }
  int j;
  for(j=0; j<24; ++j) {
    DoChar(j*4,0, 'P');
  }
}
#include "lcd/fonts/labyrinth.c"
