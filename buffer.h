#ifndef _BUFFER_H
#define	_BUFFER_H

#define BUFFER_PIENO -2
#define BUFFER_VUOTO -1
#define BUFFER_OK    0

typedef void *cell;
typedef struct buffer_t *buffer;

buffer allocBuffer(int dim);
void freeBuffer(buffer _F_buffer);
void setBufferID(buffer _buffer, int _id);
int getBufferID(buffer _buffer);
void putInBuffer_blocking(buffer _buffer, cell _cell);
int putInBuffer_not_blocking(buffer _buffer, cell _cell);
cell getFromBuffer_blocking(buffer _buffer);
cell getFromBuffer_not_blocking(buffer _buffer);
void emptyBuffer(buffer _buffer);
int isEmptyBuffer(buffer _buffer);

#endif	/* _BUFFER_H */
