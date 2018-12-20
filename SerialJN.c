/*
 * SerialJN.c
 *
 *  Created on: Mar 20, 2016
 *      Author: wesley
 */

#include "SerialJN.h"
#include "SerialGL.h"
#include "Util.h"

bool Init_NMEA_String_Flag = 0;
byte GNSS_Err_flag;
uint8_t IndexStart_GNSS = 0;
uint8_t IndexStop_GNSS = 0;
bool FlagDebugSending = 0;
char BufferRcv_GNSS[BUFFERRCV_GNSS_SIZE] = {0};

/**********************************************************
 * NAME:        SendNMEASentenceToServer
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function sends an NMEA Sentence to
 *              Server
 **********************************************************/
void SendNMEASentenceToServer(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  for(IndexStart_GNSS = 0; (char*)BufferRcv_GNSS[IndexStart_GNSS] != '\n'; IndexStart_GNSS ++){
      AS1_SendChar((char*) BufferRcv_GNSS[IndexStart_GNSS]);
  }
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/**********************************************************
 * NAME:        SendNMEASentenceToDebug
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function sends an NMEA Sentence to
 *              Serial Debug
 **********************************************************/
void SendNMEASentenceToDebug(void){
  SerialDebugSend("GPRMC:");
  IndexStart_GNSS = 1;
  AS3_SendChar((char*) BufferRcv_GNSS[0]);
  while((char*)BufferRcv_GNSS[IndexStart_GNSS] != '\n'){
      if(FlagDebugSending){
          AS3_SendChar((char*) BufferRcv_GNSS[IndexStart_GNSS]);
          FlagDebugSending = 0;
          IndexStart_GNSS ++;
      }
  }
/*  FlagDebugSending = 0;
  AS3_SendChar(0x0D);
  AS3_SendChar(0x0A);*/
  delay_ms(100);
}
/*void SendNMEASentenceToDebug(void){
  SerialDebugSend("GPRMC:");
  FlagDebugSending = 1;
  for(IndexStart_GNSS = 0; (char*)BufferRcv_GNSS[IndexStart_GNSS] != '\n'; IndexStart_GNSS ++){
      AS3_SendChar((char*) BufferRcv_GNSS[IndexStart_GNSS]);
  }
  FlagDebugSending = 0;
  AS3_SendChar(0x0D);
  AS3_SendChar(0x0A);
  delay_ms(100);
}*/

/**********************************************************
 * NAME:        StringCompareGNSS
 * INPUT:       2 pointers
 * OUTPUT:      return 0 if these pointers are equal;
 *              return -1 if these pointers aren't equal
 * DESCRIPTION: String Compare for GNSS purposes
 **********************************************************/
uint8_t StringCompareGNSS(char *Str1, char *Str2){
  int8_t Index;
  for(Index = 0; Str1[Index] || Str2[Index] == ','; Index++){
      if (ByteCompare(Str1[Index], Str2[Index]) != 0)
        return -1;
  }
  return 0;
}
