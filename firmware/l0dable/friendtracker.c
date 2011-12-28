#include <sysinit.h>
#include <string.h>

#include "basic/basic.h"
#include "basic/config.h"

#include "lcd/render.h"
#include "lcd/print.h"

#include "funk/nrf24l01p.h"
#include "usetable.h"
/*@author Steve Dierker <steve.dierker@obstkiste.org>
 * The whole thing is based on people.c and got some improvements
 * like displaying the users orderd by signal strength.
 */
void printNames(char (*usernames)[16], uint8_t * strength, uint8_t count);
uint8_t insertUser(char (*usernames)[16], uint8_t * strength, char * nuser, uint8_t nstr, uint8_t count);
void swapUsername(char * first, char * second);
int strcmp1(char * first, char * second);

void ram(void) {
    struct NRF_CFG config;
    uint8_t buf[16];
    
    // Space to save signal strength and username.
    uint8_t count = 0;
    char usernames[8][16];
    uint8_t strength[8];

    config.nrmacs=1;
    config.maclen[0] = 16;
    config.channel = 81;
    memcpy(config.mac0, "\x01\x02\x03\x02\x01", 5);
    nrf_config_set(&config);
    lcdClear();
    lcdPrintln("Track it:");
    lcdPrintln("---------");
    lcdRefresh();
    do{
        if( nrf_rcv_pkt_time(64,sizeof(buf),buf) == 16 ){
            buf[14] = 0;
            // TODO: allow last package parameter 0x25
            if( buf[1] == 0x23 || buf[1] == 0x24){
                if(0==insertUser(usernames, strength, (char *)buf+6, buf[3],
                  count)) {
                  if(count<8) ++count;
                  printNames(usernames, strength, count);
                }
            }
        }
    }while ((getInputRaw())==BTN_NONE);
}

void swapUsername(char * first, char * second) {
  char tmp[16];
  strcpy(first, tmp);
  tmp[strlen(first)-1]='\0';
  strcpy(second, first);
  first[strlen(second)-1]='\0';
  strcpy(tmp,second);
  second[strlen(tmp)-1]='\0';
}

uint8_t insertUser(char (*usernames)[16], uint8_t * strength, char * nuser, uint8_t nstr, uint8_t count) {
   int i;
   
  // Is username existing, just update strength if not add him (may delete last
  // user for it)
  uint8_t found = 0;
  for(i=0; i<count; ++i) {
    int result = strcmp1(usernames[i], nuser);
    if(0 == result) {
      strength[i] = nstr;
      found = 1;
      break;
    }
  }
  if(found==0) {
    strength[7] = nstr;
    strcpy(nuser, usernames[7]);
    usernames[7][strlen(nuser)-1] = '\0';
  }
  
  uint8_t swapped = 1;
  do {
    for(i=1; i<count; ++i) {
      if(strength[i-1]>strength[i]) {
        swapUsername(usernames[i], usernames[i-1]);
        uint8_t stmp = strength[i-1];
        strength[i-1] = strength[i];
        stmp = strength[i];
        swapped = 0;
      }
    }
  } while(swapped==0);
  return 0; 
}

void printNames(char (*usernames)[16], uint8_t * strength, uint8_t count) {
  int i;
  lcdPrintln("Track it:");
  lcdPrintln("-------------------");
  lcdPrintln(IntToStr(count,5,0));
  for(i=0; i<count; ++i) {
    lcdPrint(usernames[i]);
    lcdPrint(" strength ");
    lcdPrintln(IntToStr(strength[i],5,0));
  }
  lcdRefresh();
}

int strcmp1(char * first, char * second) {
  if(strlen(first)==strlen(second)) {
    int i;
    for(i=0; i<strlen(first);++i) {
      if(first[i]!=second[i])
        return 1;
    }
    return 0;
  }
  return 1;
}
