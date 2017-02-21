/*
 * common.h
 *
 *  Created on: 07.01.2017
 *      Author: cwiec
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <stddef.h>
#include <stdint.h>

#define BUFFER_SIZE 	64
typedef struct
{
	char buffer[BUFFER_SIZE];	//buffer
	uint8_t startIndex;			//first byte to send
	uint8_t endIntex;			//last byte to send
	uint8_t wordCount;			//amount of byte to send
} BufferS_t;

extern void cpyAndSetCleanBuf(char* current, char* last,char* cleanBuf, size_t size);

#endif /* COMMON_H_ */
