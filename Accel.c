/*
 * Accel.c
 *
 *  Created on: Mar 6, 2016
 *      Author: wesley
 */

#include "Accel.h"
#include "Util.h"
#include "SerialJN.h"

int16_t Accel_Result = 0;
int16_t Accel_XM = 0;
int16_t Accel_YM = 0;
int16_t Accel_ZM = 0;
int16_t Vetor_XM[40] = {0};
int16_t Vetor_YM[40] = {0};
int16_t Vetor_ZM[VECTOR_ACC_LENGHT] = {0};
int16_t Accel_XL = 0;
int16_t Accel_YL = 0;
int16_t Accel_ZL = 0;
extern uint16_t delay_count;
extern char BufferRcv_GNSS[BUFFERRCV_GNSS_SIZE];

/**********************************************************
 * NAME:        AnomalyCompare
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: save the higher and lesser value of a vector
 *              and estimate the road anomaly from it.
 *              Then, send the anomaly category and GNSS Data
 *              to a control server unit
 **********************************************************/
void AnomalyCompare(void){
  int8_t menor = Vetor_ZM[0];
  int8_t maior = Vetor_ZM[0];
  int8_t i;
  for(i = 1; i <= (VECTOR_ACC_LENGHT - 1); i++){
      if((Vetor_ZM[i] < Vetor_ZM[i - 1]) && (Vetor_ZM[i] <= menor)){
          menor = Vetor_ZM[i];
      }else if((Vetor_ZM[i] > Vetor_ZM[i - 1]) && (Vetor_ZM[i] >= maior)){
          maior = Vetor_ZM[i];
      }
  } 
  if(((maior-menor) >= 30) && ((maior-menor) <= 49)){
      LedAccel_NegVal();
      SerialDebugSend("\r\nANOMALIA LEVE\r\n");
      //SendANOMALIA_LEVE();
/*      while(StringCompareGNSS(GPRMC, &BufferRcv_GNSS) != 0){}
      SendNMEASentenceToDebug();*/
      //SendNMEASentenceToServer();
  }
  if(((maior-menor) >= 50) && ((maior-menor) <= 69)){
      LedGSM_NegVal();
      SerialDebugSend("\r\nANOMALIA MODERADA\r\n");
      //SendANOMALIA_MODERADA();
/*      while(StringCompareGNSS(GPRMC, &BufferRcv_GNSS) != 0){}
      SendNMEASentenceToDebug();*/
      //SendNMEASentenceToServer();
  }
  if((maior-menor) >= 70){
      LedRSSI_NegVal();
      SerialDebugSend("\r\nANOMALIA GRAVE\r\n");
      //SendANOMALIA_GRAVE();
/*      while(StringCompareGNSS(GPRMC, &BufferRcv_GNSS) != 0){}
      SendNMEASentenceToDebug();*/
      //SendNMEASentenceToServer();
  }   
}

/**********************************************************
 * NAME:        Accel_Init
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function enables Accelerometer
 *              Device to I2C Communication 
 **********************************************************/
void Accel_Init(void){
  I2C_PDD_EnableDevice(Acelerometro_DEVICE, PDD_ENABLE);
  I2C0_C1 |= I2C_C1_TX_MASK;	//Modo TX
}

/**********************************************************
 * NAME:        Accel_TriRead
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function reads 3-Axis of
 *              Accelerometer (X, Y & Z) for values been saving
 *              in Accel_-M and Accel_-L buffers which are:
 *              
 *              Accel_-M: Most significant byte
 *              Accel_-L: Lowest significant byte
 **********************************************************/
void Accel_TriRead(void){
  Accel_Read(OUT_X_MSB, &Accel_XM);
  Accel_Read(OUT_Y_MSB, &Accel_YM);
  Accel_Read(OUT_Z_MSB, &Accel_ZM);
  Accel_Read(OUT_X_LSB, &Accel_XL);
  Accel_Read(OUT_Y_LSB, &Accel_YL);
  Accel_Read(OUT_Z_LSB, &Accel_ZL);
}

/**********************************************************
 * NAME:        Accel_Read
 * INPUT:       Register: A register to be read
 *              Data: A variable to be filed with
 *                    Register contents
 * OUTPUT:      none
 * DESCRIPTION: This function reads a register from
 *              Accelerometer by I2C Communication
 **********************************************************/
void Accel_Read(uint8_t Register, int8_t *Data){
  I2C0_C1 |= I2C_C1_MST_MASK;		//START Bit
  I2C0_D = DEVICE_ADDR_WRITE;		//Envia Device Address em modo Escrita
  Wait_Flag();
  I2C0_D = Register;			//Envia Registrador a ser lido
  Wait_Flag();
  I2C0_C1 |= I2C_C1_RSTA_MASK; 	        //Repeat START
  I2C0_D = DEVICE_ADDR_READ;		//Envia Device Address em modo Leitura
  Wait_Flag();
  I2C0_C1 &= (~I2C_C1_TX_MASK);	        //Modo RX
  I2C0_C1 |= I2C_C1_TXAK_MASK;	        //Configura NACK
  *Data = I2C0_D;			//Limpa Registrador do resultado
  Wait_Flag();
  I2C0_C1 &= (~I2C_C1_MST_MASK);	//STOP Bit
  *Data = I2C0_D;			//Captura resultado
  I2C0_C1 &= (~I2C_C1_TXAK_MASK);	//Habilita Ack
  I2C0_C1 |= I2C_C1_TX_MASK;		//Modo TX
}

/**********************************************************
 * NAME:        Accel_Write
 * INPUT:       Register: A register to be write
 *              Data: A variable with data to be filed
 *              in Register
 * OUTPUT:      none
 * DESCRIPTION: This function writes a data to Register
 *              that intends to configure
 **********************************************************/
void Accel_Write(uint8_t Register, uint8_t Data){
  Accel_Write_SingleReg(0x2A, 0x00);	//Coloca em modo Standby
  Accel_Write_SingleReg(Register, Data);//Configura Registrador
  Accel_Write_SingleReg(0x2A, 0x01);	//Coloca em modo Active
}

/**********************************************************
 * NAME:        Accel_Write_SingleReg
 * INPUT:       Register: A register to be write
 *              Data: A variable with data to be filed
 *              in Register
 * OUTPUT:      none
 * DESCRIPTION: This function writes a data to Register
 *              that intends to configure
 **********************************************************/
void Accel_Write_SingleReg(uint8_t Register, uint8_t Data){
  I2C0_C1 |= I2C_C1_MST_MASK;	//START Bit
  I2C0_D = DEVICE_ADDR_WRITE;	//Envia Device Address em modo Escrita
  Wait_Flag();
  I2C0_D = Register;	//Envia Registrador a ser gravado
  Wait_Flag();
  I2C0_D = Data;	//Envia Dado a ser gravado
  Wait_Flag();
  I2C0_C1 &= (~I2C_C1_MST_MASK);        //STOP Bit
}

/**********************************************************
 * NAME:        Wait_Flag
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function waits a flag from
 *              accelerometer for indicate a completed
 *              data transmission
 **********************************************************/
void Wait_Flag(void){
  delay_count = 0;
  while((!((I2C0_S & I2C_S_IICIF_MASK) >> I2C_S_IICIF_SHIFT)) && (delay_count <= 5000));
  if(delay_count >= 5000){
      SerialDebugSend("\rI2C_Timeout\r\n");
      
      /*proximo desenvolvedor deve acrescentar o tratamento deste erro para otimizar
       * o desempenho do codigo...
       * Este erro ocorre por conta de eventual falha na comunicação entre MCU e ACC
       * no momento de depuração e/ou em desligamento repentino da fonte do protótipo.
       */
      
  }
  //delay_ms(10); //AN3468
  I2C0_S |= I2C_S_IICIF_MASK; //Limpa Flag IICIF
}

/**********************************************************
 * NAME:        Accel_TapConfig
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function configures the Accelerometer
 *              to TAP function which meaning that Device'll
 *              indicate an interrupt flag when it's
 *              detect a TAP (High Frequency) variation of
 *              G-Force.
 * REFERENCES:  AN4072 DIRECTIONAL TAP DETECTION 
 **********************************************************/
void Accel_TapConfig(void){
  Accel_Write_SingleReg(0x2A, 0x00); //Coloca em modo Standby + 800Hz ODR
  Accel_Write_SingleReg(0x21, 0x15); //Enable x,y,z Single Pulse
  Accel_Write_SingleReg(0x23, 0x30); //X_Threshold
  Accel_Write_SingleReg(0x24, 0x30); //Y_Threshold
  Accel_Write_SingleReg(0x25, 0x30); //Z_Threshold
  Accel_Write_SingleReg(0x26, 0x50); //TimeLimit for TapDetection to...
  Accel_Write_SingleReg(0x27, 0xF0); //Latency Time to...
  Accel_Write_SingleReg(0x2D, 0x08); //Enable Pulse Interrupt
  Accel_Write_SingleReg(0x2E, 0x08); //Route Pulse to INT1 HwPin
  Accel_Write_SingleReg(0x2A, 0x01); //Coloca em modo Active
}

/**********************************************************
 * NAME:        Accel_TransientConfig
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 * REFERENCES:  AN4071 TRANSIENT DETECTION
 **********************************************************/
void Accel_TransientConfig(void){
  Accel_Write_SingleReg(0x2A, 0x00); //Coloca em modo Standby + 800Hz ODR
  Accel_Write_SingleReg(0x1D, 0x18); //Enable Z flag
  Accel_Write_SingleReg(0x1F, 0x04); //Threshold to 8 counts
  Accel_Write_SingleReg(0x20, 0x05); //Debounce Counter for 50ms
  Accel_Write_SingleReg(0x2D, 0x20); //Enable Transient INT_function
  Accel_Write_SingleReg(0x2E, 0x20); //Route the INT to INT1
  Accel_Write_SingleReg(0x2A, 0x01); //Coloca em modo Active
}

/**********************************************************
 * NAME:        Accel_MotionConfig
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: -
 * REFERENCES:  AN4070 MOTION & FREEFALL DETECTION
 **********************************************************/
void Accel_MotionConfig(void){
  Accel_Write_SingleReg(0x2A, 0x18); //Coloca em modo Standby + 100Hz ODR
  Accel_Write_SingleReg(0x15, 0xD8); //Enable Motion Detection
  Accel_Write_SingleReg(0x17, 0x30); //Threshold Motion Detection of > 3g
  Accel_Write_SingleReg(0x18, 0x0A); //Debounce Counter for 100 Hz
  Accel_Write_SingleReg(0x2D, 0x04); //Enable Motion INT_function
  Accel_Write_SingleReg(0x2E, 0x04); //Route the INT to INT1
  Accel_Write_SingleReg(0x2A, 0x01); //Coloca em modo Active
}

/**********************************************************
 * NAME:        PrintAccelConfig
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function prints configurations
 *              parameter of Accelerometer on Serial Debug
 **********************************************************/
void PrintAccelConfig(void){
  SerialDebugSend("\r\n\r\n###########################\r\n");
  SerialDebugSend("ACCELEROMETER CONFIGURATION");
  SerialDebugSend("\r\n###########################\r\n\r\n");
  
  Accel_Read(XYZ_DATA_CFG, &Accel_Result);
  if (Accel_Result == 0x00){
      SerialDebugSend("Sensibility\t2G\r\nHPF\t\tDISABLED\r\n");
  }else if (Accel_Result == 0x01){
      SerialDebugSend("Sensibility\t4G\r\nHPF\t\tDISABLED\r\n");
  }else if (Accel_Result == 0x02){
      SerialDebugSend("Sensibility\t8G\r\nHPF\t\tDISABLED\r\n");
  }else if (Accel_Result == 0x10){
      SerialDebugSend("Sensibility\t2G\r\nHPF\t\tENABLED\r\n");
  }else if (Accel_Result == 0x11){
      SerialDebugSend("Sensibility\t4G\r\nHPF\t\tENABLED\r\n");
  }else if (Accel_Result == 0x12){
      SerialDebugSend("Sensibility\t8G\r\nHPF\t\tENABLED\r\n");
  }else{
      SerialDebugSend("Sensibility\tERROR\r\nHPF\t\tERROR\r\n");
  }
  delay_ms(1000);
  
  Accel_Read(CTRL_REG1, &Accel_Result);
  if (Accel_Result == 0x00){
      SerialDebugSend("Operation\tSANTBY\r\nReading\t\tNORMAL\r\nNoise\t\tNORMAL\r\nData_Rate\t800HZ\r\n");
  }else if (Accel_Result == 0x01){
      SerialDebugSend("Operation\tACTIVE\r\nReading\t\tNORMAL\r\nNoise\t\tNORMAL\r\nData_Rate\t800HZ\r\n");
  }else{
      SerialDebugSend("Operation\tERROR\r\nReading\t\tERROR\r\nNoise\t\tERROR\r\nData_Rate\tERROR\r\n");
  }
  //Next User could implement more commands here to Debug.
  delay_ms(1000);
  
  Accel_Read(WHO_AM_I, &Accel_Result); //Device_ID = 00011010
  SerialDebugSend("Accel_Address\t1D\r\n");
  delay_ms(1000);
}

/**********************************************************
 * NAME:        AccelConfig
 * INPUT:       none
 * OUTPUT:      none
 * DESCRIPTION: This function Configures accelerometer
 *              parameters for accelerometer of FRDM-KL25Z 
 **********************************************************/
void AccelConfig(void){
  Accel_Init();
  LedAccel_ClrVal();
  Accel_Write(XYZ_DATA_CFG, SENSITIVITY_8G | HPF_DISABLE);
  Accel_Write(CTRL_REG1, ACTIVE_MODE | DATA_RATE_800HZ | NORMAL_READ_MODE | NORMAL_NOISE_MODE);
  PrintAccelConfig();
  //Accel_TapConfig();
}
