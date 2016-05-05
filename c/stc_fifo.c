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
#ifdef BINARY_FIFO
void fifo_init(stc_fifo_t* fifo, void* buffer, uint16_t bitmask, uint8_t objectSize)
{
    fifo->buffer = (uint8_t*)buffer;
    fifo->objectSize = objectSize;
    fifo->bitmask = bitmask;
    fifo_clear(fifo);
}
#else
void fifo_init(stc_fifo_t* fifo, void* buffer, uint16_t buffersize, uint8_t objectSize)
{
    fifo->buffer = (uint8_t*)buffer;
    fifo->objectSize = objectSize;
    fifo->buffersize = buffersize;
    fifo_clear(fifo);
}
#endif

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
 * @brief get objects in the fifo
 *
 * Returns number of objects in the fifo.
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
 * Returns number of free objects (n * objectsize) in the fifo.
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
 * @brief returns a pointer to the nth object in the fifo
 *
 * Returns a pointer to the nth object in the fifo. If number is greater than
 * the number of objects in the fifo number will be wrapped to adress a valid object in the
 * fifo.
 * If the fifo is empty this function returns 0
 */
void* fifo_get_nth_Object(uint8_t number, stc_fifo_t* fifo)
{
	if(fifo_empty(fifo))
		return 0;

	number = number % fifo_datasize(fifo);
	return (void*)&fifo->buffer[(fifo->head + number) * fifo->objectSize];
}

/**
 * @brief deletes first object in the fifo
 *
 * Returns 1 if object has been removed, 0 if no object has been removed
 * because the fifo is empty
 */
uint8_t fifo_delete_object(stc_fifo_t* fifo)
{
	if(!fifo_empty(fifo))
	{
#ifdef BINARY_FIFO
		fifo->head = (fifo->head + 1) & fifo->bitmask;
#else
		fifo->head = (fifo->head + 1) % fifo->buffersize;
#endif
		return 1;
	}
	return 0;
}

/**
 * @brief deletes n objects in the fifo
 *
 * Returns the number of removed objects
 */
uint8_t fifo_delete_n_Objects(uint8_t number, stc_fifo_t* fifo)
{
	int i;
	for(i = 0; i < number; i++)
	{
		if(!fifo_delete_object(fifo))
		{
			break;
		}
	}

	return i;
}

/**
 * @brief read 1 byte from the fifo
 * returns 0 if fifo is empty, 1 otherwise.
 */
uint8_t fifo_read_object(void *object,stc_fifo_t* fifo)
{
	uint8_t i;
	uint8_t *memory = (uint8_t *)object;
	uint32_t fifoOffset = fifo->head * fifo->objectSize; //Calculate real number of bytes
	if(fifo_empty(fifo))
		return 0;

	for(i = 0; i < fifo->objectSize; i++)
	{
		memory[i] = fifo->buffer[fifoOffset + i];
	}
	fifo_delete_object(fifo);
	return 1;
}

/*
 * @brief read n objects from fifo
 *
 * Read arbitrary number of objects from fifo.
 * Returns the number of objects successfully read.
 */
uint32_t fifo_read(void *buffer, uint32_t number,stc_fifo_t* fifo)
{
	uint32_t i;
	uint8_t *memory = (uint8_t*)buffer;
	for (i=0; i<number; i++)
	{
		if (!fifo_read_object(&memory[i*fifo->objectSize], fifo))
			break;
	}
	return i;
}

/*
 * @brief write object in fifo
 *
 * Writes a new object into fifo.
 *
 * Returns 1 if object successfully appended to fifo, 0 otherwise.
 */
uint8_t fifo_write_object(void *object, stc_fifo_t* fifo)
{
	uint8_t i;
	uint32_t fifoOffset = fifo->tail * fifo->objectSize;
	uint8_t *memory = (uint8_t*)object;
	if(fifo_full(fifo))
		return 0;

	for(i = 0; i < fifo->objectSize; i++)
	{
		fifo->buffer[fifoOffset + i] = memory[i];
	}
#ifdef BINARY_FIFO
	fifo->tail = (fifo->tail + 1) & fifo->bitmask;
#else
	fifo->tail = (fifo->tail + 1) % fifo->buffersize;
#endif
	return 1;
}

/*
 * @brief write n objects in fifo
 *
 * Write arbitrary number of objects into fifo.
 *
 * Returns the number of objects successfully written.
 */
uint32_t fifo_write(void* buffer, uint32_t number,stc_fifo_t* fifo)
{
	uint32_t i;
	uint8_t *memory = (uint8_t *)buffer;

	for(i=0; i<number; i++)
	{
		if(!fifo_write_object(&memory[fifo->objectSize*i], fifo))
			break;
	}

	return i;
}
