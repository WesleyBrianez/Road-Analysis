/*
 * SerialGL.c
 *
 *  Created on: Mar 8, 2016
 *      Author: wesley
 */

#include "SerialGL.h"
#include "Util.h"

char BufferRcv[BUFFERRCV_SIZE] = {0};
extern uint16_t delay_count;
extern uint8_t estado;
uint8_t IndexStart = 0;
uint8_t IndexStop = 0;


/**********************************************************
 * NAME:        SendToGL865
 * INPUT:       Msg: AT Command
 * OUTPUT:      MsgSize: Size of AT Command
 * DESCRIPTION: This function sends an AT command to
 *              cellular module
 **********************************************************/
uint8_t SendToGL865(char *Msg, uint16_t MsgSize){
  uint16_t index;
  for(index = 0; index < MsgSize; index++){
      if(AS1_SendChar(Msg[index]) != ERR_OK){
          return -1;
      }
      while(!((UART1_S1 & UART_S1_TC_MASK) >> UART_S1_TC_SHIFT)){}
  }
  if(AS1_SendChar('\r') != ERR_OK){
      return -1;
  }
  return 0;
}

/**********************************************************
 * NAME:        -
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 **********************************************************/
void SendANOMALIA_LEVE(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  SendToGL865(ANOMALIA_LEVE, strlen(ANOMALIA_LEVE));
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/**********************************************************
 * NAME:        -
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 **********************************************************/
void SendANOMALIA_MODERADA(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  SendToGL865(ANOMALIA_MOD, strlen(ANOMALIA_MOD));
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/**********************************************************
 * NAME:        -
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 **********************************************************/
void SendANOMALIA_GRAVE(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  SendToGL865(ANOMALIA_GRAVE, strlen(ANOMALIA_GRAVE));
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/**********************************************************
 * NAME:        -
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 **********************************************************/
void SendCONNECTED(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  SendToGL865(CONNECTED_MSG, strlen(CONNECTED_MSG));
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/*void SendCONNECTED(void){
  AS1_SendChar('C');
  AS1_SendChar('O');
  AS1_SendChar('N');
  AS1_SendChar('N');
  AS1_SendChar('E');
  AS1_SendChar('C');
  AS1_SendChar('T');
  AS1_SendChar('E');
  AS1_SendChar('D');
  AS1_SendChar(0x1A);
}*/

/**********************************************************
 * NAME:        -
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 **********************************************************/
void SendFATECSA(void){
  SendToGL865(SSEND, strlen(SSEND));
  delay_ms(100);
  SendToGL865(FATECSA, strlen(FATECSA));
  AS1_SendChar(0x1A);
  delay_ms(100);
}

/*void SendFATECSA(void){
  AS1_SendChar('F');
  AS1_SendChar('A');
  AS1_SendChar('T');
  AS1_SendChar('E');
  AS1_SendChar('C');
  AS1_SendChar('S');
  AS1_SendChar('A');
  AS1_SendChar(0x1A);
}*/

/**********************************************************
 * NAME:        GL865Config 
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: Configures main parameters of cellular
 *              module, network and server connection 
 **********************************************************/
void GL865Config(void){
  SerialDebugSend("CELLULAR MODULE CONFIGURATION:\r\n\r\n");
  int nTries = 0;
  while(estado != CONNECTED){
      LedGSM_SetVal(); //apaga led_GSM
      switch(estado){
      case SHUTDOWN:{//VERIFICAR SE COMANDO EH SUPORTADO NO MODULO...
        SerialDebugSend("SW_REBOOT -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(SW_REBOOT, strlen(SW_REBOOT));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rSW_REBOOT -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("SW_REBOOT -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                    //IMPLEMENTAR HW_RESET
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = RESET;
                break;
            }
        }
        break;
      }
      case RESET:{
        SerialDebugSend("AT -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(ATZ, strlen(ATZ));
            delay_ms(500); //recommended delay
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rAT -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("AT -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = SIMCARD;
                break;
            }
        }
        break;
      }
      case SIMCARD:{
        SerialDebugSend("SIMCard -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(CPIN, strlen(CPIN));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rSIMCard -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("SIMCard -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = NETWORK;
                break;
            }
        }
        break;
      }
      case NETWORK:{
        SerialDebugSend("Network -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(CREG, strlen(CREG));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rNetwork -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Network -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = GPRS;
                break;
            }
        }
        break;
      }
      case GPRS:{
        SerialDebugSend("GPRS -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(CGREG, strlen(CGREG));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rGPRS -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("GPRS -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = ATTACH;
                break;
            }
        }
        break;
      }
      case ATTACH:{
        SerialDebugSend("Attaching -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(CGDCONT, strlen(CGDCONT));
            delay_ms(500); //saffety
            SendToGL865(SGACT, strlen(SGACT));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rAttaching -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Attaching -> Error: Socket ShutDown\r\n");
                    estado = SOCKCLOSE;	//SGACT=1,0
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = SOCKET;
                break;
            }
        }
        break;
      }
      case SOCKCLOSE:{
        SerialDebugSend("Socket Down -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(SH, strlen(SH));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rSocket Down -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Socket Down -> Error: Reset module\r\n");
                    estado = RESET;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = DEATTACH;
                break;
            }
        }
        break;
      }
      case DEATTACH:{
        SerialDebugSend("Deattaching -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(SGDEACT, strlen(SGDEACT));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 5000));
            if(delay_count >= 5000){
                SerialDebugSend("\rDeattaching -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Deattaching -> Error: ShutDown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = RESET;
                break;
            }
        }
        break;
      }
      case SOCKET:{
        SerialDebugSend("SocketDial -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(SD, strlen(SD));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(SRING, &BufferRcv) != 0) && (delay_count <= 10000));
            if(delay_count >= 10000){
                SerialDebugSend("\rSocketDial -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("SocketDial -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = AUTHRECV;
                //estado = FINAL;
                break;
            }
        }
        break;
      }
      case AUTHRECV:{
        SerialDebugSend("Acknowledge -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendToGL865(SRECV, strlen(SRECV));
            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 10000));
            if(delay_count >= 10000){
                SerialDebugSend("\rAcknowledge -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Acknowledge -> Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                //SerialDebugSend("OK [%d] Tries\r\n", nTries);
                SerialDebugSend("OK\r\n");
                estado = AUTHSEND;
                break;
            }
        }
        break;
      }
      case AUTHSEND:{
        SerialDebugSend("Authenticate -> ");
        for(nTries = 1; (nTries <= 4); nTries++){
            SendFATECSA(); //OK

            //SE RECEBE 0X06 = ACK

            delay_ms(500); //saffety
            delay_count = 0;
            while((StringCompare(AT_OK, &BufferRcv) != 0) && (delay_count <= 10000));
            if(delay_count >= 10000){
                SerialDebugSend("\rAuthenticate -> Timeout\r\n");
                //SerialDebugSend("NOK [%d] Tries\r\n", nTries);
                if(nTries >= 4){
                    SerialDebugSend("Authenticate -> Acknowledge Error: Shutdown module\r\n");
                    estado = SHUTDOWN;
                }
            }else{
                SerialDebugSend("\r\nCloud Connected\r\n");
                estado = CONNECTED;
                delay_ms(500);

                LedGSM_ClrVal(); //ascende Led_GSM
                SendCONNECTED();

                delay_ms(500); //saffety
                break;
            }
        }
        break;
      }
      }
  }
}
