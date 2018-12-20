/* ###################################################################
 **     Filename    : main.c
 **     Project     : Teste_I2C
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2016-03-06, 23:20, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */         
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */         
/* MODULE main */

/******************************************************
 * SEQUENCIA DOS LEDS
 * 
 * ON | GSM | ACCEL | RSSI | GPS-FIXED |
 *  |     |     |       |        |
 *  |     |     |       |        Quando Sinal GPS stiver ATIVO: Pisca a cada segundo
 *  |     |     |       RSSI Forte: Asceso Continuamente
 *  |     |     |       RSSI Mediano: Piscando
 *  |     |     |       RSSI Fraco: Apagado
 *  |     |     ACCEL_OK: Ascende
 *  |     |     ACCEL_NOK: Apagado
 *  |     Modulo GSM_OK: Ascende
 *  |     Modulo GSM_NOK: Apagado            
 *  Ao ligar o equipamento:Ascende
 *****************************************************/

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Acelerometro.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "BaseTimer.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "AS2.h"
#include "ASerialLdd2.h"
#include "Tap1.h"
#include "ExtIntLdd1.h"
#include "LedGSM.h"
#include "BitIoLdd1.h"
#include "LedRSSI.h"
#include "BitIoLdd2.h"
#include "LedAccel.h"
#include "BitIoLdd3.h"
#include "AS3.h"
#include "ASerialLdd3.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Configuracao.h"
#include "Accel.h"
#include "SerialGL.h"
#include "SerialJN.h"
#include "SerialDebug.h"

extern uint8_t estado;
extern int16_t Accel_XM;
extern int16_t Accel_YM;
extern int16_t Accel_ZM;
extern int16_t Vetor_ZM[];
extern int16_t Accel_XL;
extern int16_t Accel_YL;
extern int16_t Accel_ZL;
extern uint16_t delay_count;
extern char BufferRcv[];
extern char BufferRcv_GNSS[];
extern char GNSS_Err_flag;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  
  delay_count = 0;
  init_leds();
  estado = /*GL865;*/ MEMS; /*JN3;*/ /*DEBUG;*/

  for(;;){
      switch (estado){

      case GL865:{
        estado = RESET; //inicia a comunicação dando um soft reset no módulo
        GL865Config();  //rotina para adquirir conxeão com a ISP e servidor
        estado = MEMS;  //em seguida, o programa irá configurar o acelerômetro
        break;
      }

      case MEMS:{
        AccelConfig();
        for(;;){
            uint8_t indexACC; 
            for(indexACC = 0; indexACC <= (VECTOR_ACC_LENGHT - 1); indexACC ++){
                Accel_Read(OUT_Z_MSB, &Vetor_ZM[indexACC]);
            }
            AnomalyCompare();
        }
      }

      case DEBUG:{
        AccelConfig();
        while(1){
            Accel_TriRead();
            //SerialDebugSendInt(Accel_ZM);
            SerialDebugSend(int_to_ascii(Accel_XM));
            SerialDebugSend(",");
            SerialDebugSend(int_to_ascii(Accel_YM));
            SerialDebugSend(",");
            SerialDebugSend(int_to_ascii(Accel_ZM));
            SerialDebugSend("\r\n");
        }
      }

      case JN3:{
        SerialDebugSend("GNSS MODULE INITIALIZE\r\n\r\n");
        //AS2_EnableEvent();
        while(1){
            /*            if(GNSS_Err_flag){
                //AS2_DisableEvent();
                delay_ms(100);
                //AS2_EnableEvent();
                GNSS_Err_flag = 0;
            }*/
            if(StringCompareGNSS(GPRMC, &BufferRcv_GNSS) != 0){}
            else{
                //AS2_DisableEvent();
                SendNMEASentenceToServer();
            }
        }
      }
      }
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.09]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
