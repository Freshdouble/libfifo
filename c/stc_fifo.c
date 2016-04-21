/*
* @file stc_fifo.c
*
* Basic functions for circular buffer fifo structures
*
*  Created on: 19.04.2016
*      Author: Peter
*/

#include "stc_fifo.h"


// ===== DEFINITIONS =====

/**
 * @brief initialize fifo structure
 */
void fifo_init(stc_fifo_t* fifo)
{
	fifo->head = 0;
	fifo->tail = 0;
}

/**
 * @brief check if fifo ist empty
 * returns 1 if fifo is empty, 0 otherwise.
 */
uint8_t fifo_empty(stc_fifo_t* fifo)
{
	if(fifo->head == fifo->tail)
		return 1;
	return 0;
}

/*
 * @check if fifo ist full
 *
 * Returns 1 if fifo is full, 0 otherwise.
 */
uint8_t fifo_full(stc_fifo_t* fifo)
{
	if(((fifo->tail + 1) % FIFO_BUFFER_SIZE) == fifo->head)
	{
		return 1;
	}
	return 0;
}

/**
 * @brief read 1 char from the fifo
 * returns 0 if fifo is empty, 1 otherwise.
 */
uint8_t fifo_read_char(char *c,stc_fifo_t* fifo)
{
	if(fifo_empty(fifo))
		return 0;

	*c = fifo->buffer[fifo->head];
	fifo->head = (fifo->head + 1) % FIFO_BUFFER_SIZE;
	return 1;
}

/*
 * @brief read n bytes from fifo
 *
 * Read arbitrary number of chars from fifo.
 * Returns the number of characters successfully read.
 */
uint32_t fifo_read(uint8_t *buffer, uint32_t number,stc_fifo_t* fifo)
{
	uint32_t i;
	for (i=0; i<number; i++)
	{
		if (!fifo_read_char((char *)(buffer + i), fifo))
			break;
	}
	return i;
}

/*
 * @brief write char in fifo
 *
 * Writes a new char into fifo.
 *
 * Returns 1 if char successfully appended to fifo, 0 otherwise.
 */
uint8_t fifo_write_char(char c, stc_fifo_t* fifo)
{
	if(fifo_full(fifo))
		return 0;

	fifo->buffer[fifo->tail] = c;
	fifo->tail = (fifo->tail + 1) % FIFO_BUFFER_SIZE;
	return 1;
}

/*
 * @brief write n chars in fifo
 *
 * Write arbitrary number of chars into fifo.
 *
 * Returns the number of chars successfully written.
 */
uint32_t fifo_write(char* buffer, uint32_t number,stc_fifo_t* fifo)
{
	uint32_t i;
	for(i=0; i<number; i++)
	{
		if(!fifo_write_char(buffer[i], fifo))
			break;
	}

	return i;
}
