/*
 * Util.h
 *
 *  Created on: Mar 6, 2016
 *      Author: wesley
 */

#ifndef UTIL_H_
#define UTIL_H_

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Acelerometro.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "I2C_PDD.h"

uint8_t StringCompare(char *Str1, char *Str2);
uint8_t ByteCompare(char x, char y);
char *int_to_ascii(int num);
void delay_ms(uint16_t time);
void init_leds(void);
void quick_sort(int *a, int left, int right);

#define GL865		        12
#define MEMS		        13
#define JN3		        14
#define DEBUG                   15

#define BUFFERRCV_GNSS_SIZE     127
#define BUFFERRCV_SIZE          127
#define NMEA_SENTENCE_SIZE      5

#endif /* UTIL_H_ */
