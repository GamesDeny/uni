//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "buffer_t.h"

#include <stdio.h>
#include <stdlib.h>

buffer_t *buffer_init(unsigned int maxsize) {
    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));

    buffer->messages = (msg_t **) malloc(maxsize * sizeof(msg_t *));
    buffer->get_index = 0;
    buffer->put_index = 0;
    buffer->count = 0;
    buffer->maxsize = maxsize;

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->full), NULL);
    pthread_cond_init(&(buffer->empty), NULL);

    return buffer;
}

void buffer_destroy(buffer_t *buffer) {
    check(buffer != NULL, "buffer_destroy() - Null buffer found...");

    free(buffer->messages);

    pthread_mutex_destroy(&(buffer->mutex));

    pthread_cond_destroy(&(buffer->full));
    pthread_cond_destroy(&(buffer->empty));

    free(buffer);
}

msg_t *blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "blocking_put() - Null buffer found...");
    check(msg != NULL, "blocking_put() - Null msg found...");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == buffer->maxsize) {
        pthread_cond_wait(&(buffer->full), &(buffer->mutex));
    }

    buffer->messages[buffer->get_index] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->get_index = (buffer->get_index + 1) % buffer->maxsize;
    buffer->count++;

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;

}

msg_t *non_blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");
    check(msg != NULL, "non_blocking_put() - Null msg found...");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == buffer->maxsize) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    buffer->messages[buffer->get_index] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->get_index = (buffer->get_index + 1) % buffer->maxsize;
    buffer->count++;

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

msg_t *blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == 0) {
        pthread_cond_wait(&(buffer->empty), &(buffer->mutex));
    }

    msg_t *msg = buffer->messages[buffer->put_index];
    buffer->put_index = (buffer->put_index + 1) % buffer->maxsize;
    buffer->count--;

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

msg_t *non_blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == 0) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    msg_t *msg = buffer->messages[buffer->put_index];
    buffer->put_index = (buffer->put_index + 1) % buffer->maxsize;
    buffer->count--;

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

