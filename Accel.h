/*
 * Accel.h
 *
 *  Created on: Mar 6, 2016
 *      Author: wesley
 */

#ifndef ACCEL_H_
#define ACCEL_H_

#include "Util.h"


/****************************************************************************
 * DEFINITIONS
 ***************************************************************************/
#define		DEVICE_ADDR_WRITE		0x3A
#define		DEVICE_ADDR_READ		0x3B
#define         CONST_ACCELERATION              9,8
#define         VECTOR_ACC_LENGHT               40


/****************************************************************************
 * FUNCTIONS
 ***************************************************************************/
void Wait_Flag(void);
void Accel_Write_SingleReg(uint8_t Register, uint8_t Data);
void Accel_Init(void);
void Accel_TriRead(void);
void Accel_Read(uint8_t Register, int8_t *Data);
void Accel_Write(uint8_t Register, uint8_t Data);
void AccelConfig(void);
void Accel_TapConfig(void);
void Accel_MotionConfig(void);
void Accel_TransientConfig(void);
void PrintAccelConfig(void);
void AnomalyCompare(void);


/****************************************************************************
 * ACCELEROMETER REGISTERS
 ***************************************************************************/

//REGISTERS ADDRESS
#define 	WHO_AM_I			0x0Du
#define 	CTRL_REG1			0x2Au
#define 	XYZ_DATA_CFG    		0x0Eu

#define 	OUT_X_MSB			0x01u
#define 	OUT_X_LSB			0x02u
#define 	OUT_Y_MSB			0x03u
#define 	OUT_Y_LSB			0x04u
#define 	OUT_Z_MSB			0x05u
#define 	OUT_Z_LSB			0x06u

//XYZ_DATA_CFG Bit Fields 
#define         SENSITIVITY_8G                  0x02u
#define         SENSITIVITY_4G                  0x01u
#define         SENSITIVITY_2G                  0x00u
#define         HPF_DISABLE                     0x00u
#define         HPF_ENABLE                      0x10u

//CTRL_REG1 Bit Fields
#define         ACTIVE_MODE                     0x01u
#define         STANDBY_MODE                    0x00u
#define         NORMAL_READ_MODE                0x00u
#define         FAST_READ_MODE                  0x02u
#define         NORMAL_NOISE_MODE               0x00u
#define         REDUCED_NOISE_MODE              0x03u

//ODR is the speed of acc-reading = Output Data Rate/Read
#define         DATA_RATE_800HZ                 0x00u
#define         DATA_RATE_400HZ                 0x08u
#define         DATA_RATE_200HZ                 0x10u
#define         DATA_RATE_100HZ                 0x18u
#define         DATA_RATE_50HZ                  0x20u
#define         DATA_RATE_12HZ                  0x28u   //12.5HZ
#define         DATA_RATE_62HZ                  0x30u   //6.25HZ
#define         DATA_RATE_1HZ                   0x38u   //1.56HZ

#endif /* ACCEL_H_ */
