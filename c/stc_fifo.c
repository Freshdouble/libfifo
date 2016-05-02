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
void fifo_init(stc_fifo_t* fifo,void* buffer)
{
    fifo->buffer = (uint8_t*)buffer;
    fifo_clear(fifo);
}

void fifo_clear(stc_fifo_t* fifo)
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
#ifdef BINARY_FIFO
	if(((fifo->tail + 1) & fifo->bitmask) == fifo->head)
	{
		return 1;
	}
#else
	if(((fifo->tail + 1) % fifo->buffersize) == fifo->head)
	{
		return 1;
	}
#endif
	return 0;
}

/*
 * @brief get bytes in the fifo
 *
 * Returns number of bytes in the fifo.
 */
uint16_t fifo_datasize(stc_fifo_t* fifo)
{
	int size;
	size = fifo->head - fifo->tail;
	if(size < 0)
	{
#ifdef BINARY_FIFO
		size += fifo->bitmask + 1;
#else
		size += fifo->buffersize;
#endif
	}
	return (uint16_t)size;
}

/*
 * @brief get free space of fifo
 *
 * Returns number of free bytes in the fifo.
 */
uint16_t fifo_free_space(stc_fifo_t* fifo)
{
#ifdef BINARY_FIFO
	return (fifo->bitmask + 1) - fifo_datasize(fifo);
#else
	return fifo->buffersize - fifo_datasize(fifo);
#endif
}

/**
 * @brief read 1 char from the fifo
 * returns 0 if fifo is empty, 1 otherwise.
 */
uint8_t fifo_read_char(uint8_t *c,stc_fifo_t* fifo)
{
	if(fifo_empty(fifo))
		return 0;

	*c = fifo->buffer[fifo->head];
#ifdef BINARY_FIFO
	fifo->head = (fifo->head + 1) & fifo->bitmask;
#else
	fifo->head = (fifo->head + 1) % fifo->buffersize;
#endif
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
		if (!fifo_read_char((uint8_t *)(buffer + i), fifo))
			break;
	}
	return i;
}

/*
 * @brief read an user defined object from the buffer
 *
 * Returns the number of bytes successfully read.
 */
uint32_t fifo_read_object(void *object, uint32_t size, stc_fifo_t* fifo)
{
	uint8_t *buffer = (uint8_t *)object;

	if(fifo_datasize(fifo) < size)
		return 0;

	return fifo_read(buffer, size, fifo);
}

/*
 * @brief write char in fifo
 *
 * Writes a new char into fifo.
 *
 * Returns 1 if char successfully appended to fifo, 0 otherwise.
 */
uint8_t fifo_write_char(uint8_t c, stc_fifo_t* fifo)
{
	if(fifo_full(fifo))
		return 0;

	fifo->buffer[fifo->tail] = c;
#ifdef BINARY_FIFO
	fifo->tail = (fifo->tail + 1) & fifo->bitmask;
#else
	fifo->tail = (fifo->tail + 1) % fifo->buffersize;
#endif
	return 1;
}

/*
 * @brief write n chars in fifo
 *
 * Write arbitrary number of chars into fifo.
 *
 * Returns the number of chars successfully written.
 */
uint32_t fifo_write(uint8_t* buffer, uint32_t number,stc_fifo_t* fifo)
{
	uint32_t i;
	for(i=0; i<number; i++)
	{
		if(!fifo_write_char(buffer[i], fifo))
			break;
	}

	return i;
}

/*
 * @brief write a user defined object to the buffer
 *
 * Returns the number of bytes successfully written.
 */
uint32_t fifo_write_data(void* data, uint32_t number, stc_fifo_t *fifo)
{
	uint8_t *buffer = (uint8_t *)data;

	if(fifo_free_space(fifo) < number)
		return 0;

	return fifo_write(buffer,number,fifo);
}
