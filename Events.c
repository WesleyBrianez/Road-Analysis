/* ###################################################################
 **     Filename    : Events.c
 **     Project     : Teste_I2C
 **     Processor   : MKL25Z128VLK4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2016-03-06, 23:20, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Settings    :
 **     Contents    :
 **         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */         
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Util.h"
#include "SerialGL.h"
#include "SerialJN.h"

#ifdef __cplusplus
extern "C" {
#endif 


  /* User includes (#include below this line is not maintained by Processor Expert) */
  
  extern uint8_t IndexStart;
  extern uint8_t IndexStop;
  extern char BufferRcv[];
  extern bool FlagDebugSending;
  
  extern uint8_t IndexStart_GNSS;
  extern byte GNSS_Err_flag;
  extern char BufferRcv_GNSS[];
  extern uint8_t IndexStop_GNSS;
  

  /*
   ** ===================================================================
   **     Event       :  Cpu_OnNMIINT (module Events)
   **
   **     Component   :  Cpu [MKL25Z128LK4]
   */
  /*!
   **     @brief
   **         This event is called when the Non maskable interrupt had
   **         occurred. This event is automatically enabled when the [NMI
   **         interrupt] property is set to 'Enabled'.
   */
  /* ===================================================================*/
  void Cpu_OnNMIINT(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  BaseTimer_OnInterrupt (module Events)
   **
   **     Component   :  BaseTimer [TimerInt]
   **     Description :
   **         When a timer interrupt occurs this event is called (only
   **         when the component is enabled - <Enable> and the events are
   **         enabled - <EnableEvent>). This event is enabled only if a
   **         <interrupt service/event> is enabled.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void BaseTimer_OnInterrupt(void)
  {
    /* Write your code here ... */
    extern uint16_t delay_count;
    delay_count++;
  }

  /*
   ** ===================================================================
   **     Event       :  AS1_OnError (module Events)
   **
   **     Component   :  AS1 [AsynchroSerial]
   **     Description :
   **         This event is called when a channel error (not the error
   **         returned by a given method) occurs. The errors can be read
   **         using <GetError> method.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS1_OnError(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS1_OnRxChar (module Events)
   **
   **     Component   :  AS1 [AsynchroSerial]
   **     Description :
   **         This event is called after a correct character is received.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled and either the <Receiver>
   **         property is enabled or the <SCI output mode> property (if
   **         supported) is set to Single-wire mode.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS1_OnRxChar(void)
  {
    /* Write your code here ... */
    AS1_RecvChar(&BufferRcv[IndexStop]);	//save the variable in BufferRcv[IndexStop]
    if(((char*) BufferRcv[IndexStop]) == '\r'){	//1º \r come to BufferRcv: INIT of String & Command Echo
        for(IndexStart = 0; IndexStart <= IndexStop; IndexStart++){}
        IndexStart = 0;
        IndexStop = 0;
    }
    else{
        IndexStop++;		
    }
  }

  /*
   ** ===================================================================
   **     Event       :  AS1_OnTxChar (module Events)
   **
   **     Component   :  AS1 [AsynchroSerial]
   **     Description :
   **         This event is called after a character is transmitted.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS1_OnTxChar(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS1_OnFullRxBuf (module Events)
   **
   **     Component   :  AS1 [AsynchroSerial]
   **     Description :
   **         This event is called when the input buffer is full;
   **         i.e. after reception of the last character 
   **         that was successfully placed into input buffer.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS1_OnFullRxBuf(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS1_OnFreeTxBuf (module Events)
   **
   **     Component   :  AS1 [AsynchroSerial]
   **     Description :
   **         This event is called after the last character in output
   **         buffer is transmitted.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS1_OnFreeTxBuf(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS2_OnError (module Events)
   **
   **     Component   :  AS2 [AsynchroSerial]
   **     Description :
   **         This event is called when a channel error (not the error
   **         returned by a given method) occurs. The errors can be read
   **         using <GetError> method.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS2_OnError(void)
  {
    /* Write your code here ... */
    AS2_GetError(&GNSS_Err_flag);
  }

  /*
   ** ===================================================================
   **     Event       :  AS2_OnRxChar (module Events)
   **
   **     Component   :  AS2 [AsynchroSerial]
   **     Description :
   **         This event is called after a correct character is received.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled and either the <Receiver>
   **         property is enabled or the <SCI output mode> property (if
   **         supported) is set to Single-wire mode.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS2_OnRxChar(void)
  {
    /* Write your code here ... */
    if(FlagDebugSending == 0){
        AS2_RecvChar(&BufferRcv_GNSS[IndexStop_GNSS]);    //receive a char in AS2 and save it in BufferRcv
        if(((char*) BufferRcv_GNSS[IndexStop_GNSS]) == '\n'){
            for(IndexStart_GNSS = 0; IndexStart_GNSS <= IndexStop_GNSS; IndexStart_GNSS++){}
            IndexStart_GNSS = 0;
            IndexStop_GNSS = 0;
        }else{
            IndexStop_GNSS++;
        }
    }
  }

  /*
   ** ===================================================================
   **     Event       :  AS2_OnFullRxBuf (module Events)
   **
   **     Component   :  AS2 [AsynchroSerial]
   **     Description :
   **         This event is called when the input buffer is full;
   **         i.e. after reception of the last character 
   **         that was successfully placed into input buffer.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS2_OnFullRxBuf(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  Tap1_OnInterrupt (module Events)
   **
   **     Component   :  Tap1 [ExtInt]
   **     Description :
   **         This event is called when an active signal edge/level has
   **         occurred.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void Tap1_OnInterrupt(void)
  {
    /* Write your code here ... */
    bool FlagTap1 = 1;
  }

  /*
   ** ===================================================================
   **     Event       :  AS3_OnError (module Events)
   **
   **     Component   :  AS3 [AsynchroSerial]
   **     Description :
   **         This event is called when a channel error (not the error
   **         returned by a given method) occurs. The errors can be read
   **         using <GetError> method.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS3_OnError(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS3_OnRxChar (module Events)
   **
   **     Component   :  AS3 [AsynchroSerial]
   **     Description :
   **         This event is called after a correct character is received.
   **         The event is available only when the <Interrupt
   **         service/event> property is enabled and either the <Receiver>
   **         property is enabled or the <SCI output mode> property (if
   **         supported) is set to Single-wire mode.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS3_OnRxChar(void)
  {
    /* Write your code here ... */
  }

  /*
   ** ===================================================================
   **     Event       :  AS3_OnTxChar (module Events)
   **
   **     Component   :  AS3 [AsynchroSerial]
   **     Description :
   **         This event is called after a character is transmitted.
   **     Parameters  : None
   **     Returns     : Nothing
   ** ===================================================================
   */
  void AS3_OnTxChar(void)
  {
    /* Write your code here ... */
  }

/*
** ===================================================================
**     Event       :  AS3_OnTxComplete (module Events)
**
**     Component   :  AS3 [AsynchroSerial]
**     Description :
**         This event indicates that the transmitter is finished
**         transmitting all data, preamble, and break characters and is
**         idle. It can be used to determine when it is safe to switch
**         a line driver (e.g. in RS-485 applications).
**         The event is available only when both <Interrupt
**         service/event> and <Transmitter> properties are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS3_OnTxComplete(void)
{
  /* Write your code here ... */
  FlagDebugSending = 1;
}

  /* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
