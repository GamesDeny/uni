//
// Created by francesco_pio_montrano on 08/06/23.
//

#ifndef UNI_MSGT_H
#define UNI_MSGT_H


typedef struct {
    char *content;

    // generico contenuto del msg
    struct msg_t *(*msg_init)(void *);

    // creazione msg
    void (*msg_destroy)(struct msg_t *);

    // deallocazione msg
    struct msg_t *(*msg_copy)(struct msg_t *); // creazione/copia msg
} msg_t;

msg_t *msg_init(void *content);

void msg_destroy(msg_t *msg_t);

msg_t *msg_copy(msg_t *msg_t);

#endif //UNI_MSGT_H
