/*
 * @file stc_fifo.h
 *
 * Includes basic structures to use with circular buffer fifo structures
 *
 *  Created on: 19.04.2016
 *  Author: Peter
 */

#ifndef STC_FIFO_H_
#define STC_FIFO_H_

// ===== INCLUDES =====
#include <stdint.h>

//#define BINARY_FIFO

// ===== STRUCTS =====

typedef struct stc_fifo_s
{
	uint8_t *buffer;
	uint16_t head;
	uint16_t tail;
	uint8_t objectSize;
#ifdef BINARY_FIFO
	uint16_t bitmask;
#else
	uint16_t buffersize;
#endif
}stc_fifo_t;

// ===== DECLARATIONS =====

void fifo_init(stc_fifo_t* fifo,void* buffer);
void fifo_clear(stc_fifo_t* fifo);
uint8_t fifo_empty(stc_fifo_t* fifo);
uint8_t fifo_full(stc_fifo_t* fifo);
uint16_t fifo_datasize(stc_fifo_t* fifo);
uint16_t fifo_free_space(stc_fifo_t* fifo);
uint8_t fifo_read_object(void *object,stc_fifo_t* fifo);
uint32_t fifo_read(void *buffer, uint32_t number,stc_fifo_t* fifo);
uint8_t fifo_write_object(void *object, stc_fifo_t* fifo);
uint32_t fifo_write(void* buffer, uint32_t number,stc_fifo_t* fifo);

#endif
