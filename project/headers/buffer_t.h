//
// Created by francesco_pio_montrano on 08/06/23.
//
#ifndef BUFFER_H
#define BUFFER_H

#include "msg_t.h"
#include <pthread.h>

#define BUFFER_ERROR (msg_t*) NULL

#include <stdatomic.h>

typedef struct {
    msg_t **messages;
    atomic_ulong get_index;
    atomic_ulong put_index;
    atomic_ulong count;
    unsigned int maxsize;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
} buffer_t;


/*
 * Allocazione / deallocazione buffer
 * creazione di un buffer vuoto di dim. max nota
 */
buffer_t *buffer_init(unsigned int maxsize);

/*
* deallocazione di un buffer
*/
void buffer_destroy(buffer_t *buffer);

/*
* inserimento bloccante
* sospende se pieno, quindi effettua l’inserimento non appena si libera dello spazio
*
* return:
*  ERROR se msg == null o errori generici
*  messaggio inserito
*/
msg_t *blocking_put(buffer_t *buffer, msg_t *msg);

/*
* inserimento non bloccante
*
* return:
*  BUFFER_ERROR se pieno o msg == null
*  effettua l’inserimento e restituisce il messaggio
*
*/
msg_t *non_blocking_put(buffer_t *buffer, msg_t *msg);

/*
* estrazione bloccante
* sospende se vuoto
*
* return:
*  il valore estratto non appena disponibile
*
*/
msg_t *blocking_get(buffer_t *buffer);

/*
* estrazione non bloccante
*
* return:
*  BUFFER_ERROR se vuoto
*  valore estratto
*
*/
msg_t *non_blocking_get(buffer_t *buffer);

#endif /* BUFFER_H */