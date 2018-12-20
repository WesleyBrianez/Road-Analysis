/*
 * SerialDebug.h
 *
 *  Created on: Jun 21, 2016
 *      Author: wesley
 */

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

/****************************************************************************
 * DEFINITIONS
 ***************************************************************************/

/****************************************************************************
 * FUNCTIONS
 ***************************************************************************/
void SerialDebugEnable(void);
void SerialDebugDisable(void);
void SerialDebugSend(char *msg);
void SerialDebugSendInt(short int msg);

#endif /* SERIALDEBUG_H_ */
