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

// ===== DEFINES =====
#define FIFO_BUFFER_SIZE 256

// ===== STRUCTS =====

typedef struct stc_fifo_s
{
	char buffer[FIFO_BUFFER_SIZE];
	uint16_t head;
	uint16_t tail;
}stc_fifo_t;

// ===== DECLARATIONS =====

void fifo_init(stc_fifo_t* fifo);
uint8_t fifo_empty(stc_fifo_t* fifo);
uint8_t fifo_full(stc_fifo_t* fifo);
uint8_t fifo_read_char(char *c,stc_fifo_t* fifo);
uint32_t fifo_read(uint8_t *buffer, uint32_t number,stc_fifo_t* fifo);
uint8_t fifo_write_char(char c, stc_fifo_t* fifo);
uint32_t fifo_write(char* buffer, uint32_t number,stc_fifo_t* fifo);

#endif
