/*
 * Util.c
 *
 *  Created on: Mar 6, 2016
 *      Author: wesley
 */


#include "Util.h"
#include "SerialGL.h"
#include "SerialJN.h"

/************************************************
 * VARIABLES TO GENERAL PURPOSES
 ************************************************/
uint16_t delay_count;
uint8_t estado;

/************************************************
 * NAME:        delay_ms
 * INPUT:       0 to 65536 value
 * OUTPUT:      none
 * DESCRIPTION: general purpose delays 
 ************************************************/
void delay_ms(uint16_t time){
	delay_count = 0;
	while(delay_count <= time){}
}

/************************************************
 * NAME:        init_leds
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: indicate initialization of project
 *              by 3 LED's. It's delayd 500ms as
 *              recommended on GL865_HWUserGuide 
 ************************************************/
void init_leds(void){
  LedGSM_ClrVal();      //ascende LED GSM
  LedAccel_ClrVal();    //ascende LED Accel
  LedRSSI_ClrVal();     //ascende LED RSSI
  delay_ms(1000);       //recommended at least 0,5s from GL865_HardwareUserGuide - pg16
  LedGSM_SetVal();      //apaga LED GSM
  LedAccel_SetVal();    //apaga LED Accel
  LedRSSI_SetVal();     //apaga LED RSSI
}
/**************************************************
 * NAME:        int_to_ascii
 * INPUT:       integer number: -127 to +127
 * OUTPUT:      returns an character wich
 *              represents an ASCII of INPUT number
 * DESCRIPTION: ASCII representatives of a number
 *              for printing final variable on
 *              serial terminal for example.
 **************************************************/
char *int_to_ascii(int num){
  static char str[11];
  char *ptr = &str[10];
  if(!num) 
    *--ptr = 0x30;
  else{
      if((num >> 7) & 0x01){ //signal verify
          num = (~num) & 0x00FF;
          for(;num;num/=10){
              *--ptr = 0x30 + num%10;
          }
          *--ptr = 0x2d; //put the '-' signal
      }else{
          for(;num;num/=10){
              *--ptr = 0x30 + num%10;
          }
      }
  }
  return ptr;
}

/**************************************************
 * NAME:        StringCompare
 * INPUT:       2 pointers
 * OUTPUT:      return 0 if these pointers are equal;
 *              return -1 if thes pointers aren't equal
 * DESCRIPTION: String Compare for general purposes
 **************************************************/
uint8_t StringCompare(char *Str1, char *Str2){
  int8_t Index;
  for(Index = 0; Str1[Index] || Str2[Index] != '\r'; Index++){
      if (ByteCompare(Str1[Index], Str2[Index]) != 0)
        return -1;
  }
  return 0;
}

/**************************************************
 * NAME:        ByteCompare
 * INPUT:       2 characters
 * OUTPUT:      return 0 if these chars are equal;
 *              return -1 if these chars aren't equal
 * DESCRIPTION: Byte Compare for general purposes
 **************************************************/
uint8_t ByteCompare(char x, char y){
  if(x == y)
    return 0;
  else
    return -1;
}

/************************************************
 *      FUNCTIONS CREATED FOR CHANGE CONSOLE
 *      ISSUE TO MAKE A DEBUG TROUGHT AS3-SERIAL
 ************************************************/
void InitializeUART(void){}
void ReadUARTN(void){}
void WriteUARTN(void){}
void sys_exit(void){}
