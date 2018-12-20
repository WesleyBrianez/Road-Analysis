/*
 * SerialDebug.c
 *
 *  Created on: Jun 21, 2016
 *      Author: wesley
 */

#include "SerialDebug.h"
#include "Util.h"

/**********************************************************
 * NAME:        SerialDebugEnable
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function enables a serial port for
 *              project debugging 
 **********************************************************/
void SerialDebugEnable(void){
  AS3_Enable();
  AS3_EnableEvent();
}

/**********************************************************
 * NAME:        SerialDebugDisable
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function disables serial debugger
 **********************************************************/
void SerialDebugDisable(void){
  AS3_DisableEvent();
  AS3_Disable();
}

/**********************************************************
 * NAME:        SerialDebugSend
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function sends an ASCII data
 *              to serial port
 **********************************************************/
void SerialDebugSend(char *msg){
  uint16_t index;
  for(index = 0; msg[index] != '\0'; index++){
      AS3_SendChar(msg[index]);
      while(!((UART0_S1 & UART_S1_TC_MASK) >> UART_S1_TC_SHIFT)){}
  }
}

/**********************************************************
 * NAME:        SerialDebugSendInt
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function sends an integer data to
 *              serial port
 **********************************************************/
void SerialDebugSendInt(short int msg){
  AS3_SendChar(msg);
}
