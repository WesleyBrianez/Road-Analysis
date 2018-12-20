/*
 * SerialGL.h
 *
 *  Created on: Mar 8, 2016
 *      Author: wesley
 */

#ifndef SERIALGL_H_
#define SERIALGL_H_

#include "Util.h"

/****************************************************************************
 * DEFINITIONS
 ***************************************************************************/
#define AT_OK		"\nOK"			//Attention command OK
#define AT_ER		"\nERROR"
#define ATZ		"ATZ"			//Soft Reset
#define ATE0		"ATE0"			//Echo Disable ????????
#define AT		"AT"			//Attention Test
#define CPIN		"AT+CPIN?"		//SIMCard Verify
#define CPIN_READY	"\n+CPIN: READY"//SIMCard Verify
#define CIMI		"AT+CIMI"		//SIMCard IMSI
#define CREG		"AT+CREG?"		//Network Registration Config
#define CREG_01		"\n+CREG: 0,1"
#define CGREG		"AT+CGREG?"		//GPRS Network Registration
#define CGREG_01	"\n+CGREG: 0,1"
#define CGATT		"AT+CGATT?"		//GPRS Attach
#define CGATT_1		"\n+CGATT: 1"
#define SW_REBOOT       "\nAT#REBOOT"
#define CONNECTED_MSG   "CONNECTED"
#define ANOMALIA_LEVE  "ANOMALIA LEVE DETECTADA"
#define ANOMALIA_MOD   "ANOMALIA MODERADA DETECTADA"
#define ANOMALIA_GRAVE "ANOMALIA GRAVE DETECTADA"

//#define SGACT		"AT#SGACT=1,1,\"VIVO\",\"VIVO\""
#define SGACT           "AT#SGACT=1,1"
#define SGDEACT		"AT#SGACT=1,0"
#define SH		"AT#SH=1"
#define SD		"AT#SD=1,0,1120,\"186.201.179.118\",0,0,1"
//#define SD              "AT#SD=1,0,1120,\"201.83.254.40\",0,0,1"
#define SRING		"\nSRING: 1"

#define SRECV		"AT#SRECV=1,32"
#define Auth		"\nAuth-Required"

#define SSEND		"AT#SSEND=1"
#define FATECSA		"FATECSA"
#define TESTE		"testeTCC"

//#define CGDCONT		"AT+CGDCONT=1,\"IP\",\"zap.vivo.com.br\""
#define CGDCONT         "AT+CGDCONT=1,\"IP\",\"JAVA.CLARO.COM.BR\""
#define CSQ		"AT+CSQ"		//Signal Quality (dbm)
#define CSCA		"AT+CSCA?"		//SIMCard phone number
#define COPS		"AT+COPS=3,2"	//Operator Selection
#define GMM		"AT+GMM"		//Model Identification
#define SII		"AT#SII=2"		//
#define CGSN		"AT+CGSN"		//Serial Number - IMEI
#define CGMR		"AT+CGMR"		//Rev. Identification

#define SHUTDOWN	0
#define RESET		1
#define SIMCARD		2
#define NETWORK		3
#define GPRS		4
#define ATTACH		5
#define SOCKCLOSE	6
#define DEATTACH	7
#define SOCKET		8
#define AUTHRECV	9
#define AUTHSEND	10
#define CONNECTED	11

/****************************************************************************
 * FUNCTIONS
 ***************************************************************************/
uint8_t SendToGL865(char *Msg, uint16_t MsgSize);
void GL865Config(void);
void SendCONNECTED(void);
void SendFATECSA(void);

#endif /* SERIALGL_H_ */

