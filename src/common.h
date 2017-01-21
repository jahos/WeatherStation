/*
 * common.h
 *
 *  Created on: 07.01.2017
 *      Author: cwiec
 */

#ifndef COMMON_H_
#define COMMON_H_

#define BUFFER_SIZE 	64
typedef struct
{
	char buffer[BUFFER_SIZE];	//buffer
	uint8_t startIndex;			//first byte to send
	uint8_t endIntex;			//last byte to send
	uint8_t wordCount;			//amount of byte to send
} BufferS_t;

#endif /* COMMON_H_ */
