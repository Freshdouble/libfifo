/*
 * @file libfifo.h
 *
 * Includes basic structures to use with circular buffer fifo structures
 *
 *  Created on: 19.04.2016
 *  Author: Peter
 */

#ifndef LIBFIFO_H_
#define LIBFIFO_H_

// ===== INCLUDES =====
#include <stdint.h>

//#define BINARY_FIFO
#define GET_BUFFER_SIZE(realsize) (realsize + 1)

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
} fifo_t;

// ===== DECLARATIONS =====

char fifo_init(fifo_t* fifo, void* buffer, uint16_t buffersize,uint8_t objectSize);
void fifo_clear(fifo_t* fifo);
uint8_t fifo_empty(fifo_t* fifo);
uint8_t fifo_full(fifo_t* fifo);
uint16_t fifo_datasize(fifo_t* fifo);
uint16_t fifo_free_space(fifo_t* fifo);
void* fifo_get_nth_Object(uint8_t number, fifo_t* fifo);
uint8_t fifo_delete_object(fifo_t* fifo);
uint8_t fifo_delete_n_Objects(uint8_t number, fifo_t* fifo);
uint8_t fifo_read_object(void *object, fifo_t* fifo);
uint32_t fifo_read(void *buffer, uint32_t number, fifo_t* fifo);
uint8_t fifo_write_object(void *object, fifo_t* fifo);
uint32_t fifo_write(void* buffer, uint32_t number, fifo_t* fifo);

#endif
