//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <stdio.h>
#include <stdlib.h>

buffer_t *buffer_init(unsigned long maxsize) {
    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));

    buffer->messages = malloc(sizeof(msg_t *) * buffer->maxsize);
    atomic_store(&buffer->count, 0);
    buffer->maxsize = maxsize;

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->full), NULL);
    pthread_cond_init(&(buffer->empty), NULL);

    return buffer;
}

void buffer_destroy(buffer_t *buffer) {
    check(buffer != NULL, "buffer_destroy() - Null buffer found\n");

    for (int i = 0; i < buffer->count; i++) {
        printf("buffer_destroy() - Messages is not empty so freeing msg: %d\n", i);
        free(buffer->messages[i]);
    }
    free(buffer->messages);

    pthread_mutex_destroy(&(buffer->mutex));

    pthread_cond_destroy(&(buffer->full));
    pthread_cond_destroy(&(buffer->empty));

    free(buffer);
}

msg_t *blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "blocking_put() - Null buffer found\n");
    check(msg != BUFFER_ERROR, "blocking_put() - Null msg found\n");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == buffer->maxsize) {
        pthread_cond_wait(&(buffer->full), &(buffer->mutex));
    }

    buffer->messages[buffer->count] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->count = atomic_fetch_add(&buffer->count, 1) % buffer->maxsize;
    printf("blocking_put() - Buffer count: %lu\n", buffer->count);

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "blocking_put() - Returning NULL msg\n");

    return msg;
}

msg_t *non_blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found\n");
    check(msg != BUFFER_ERROR, "non_blocking_put() - Null msg found\n");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == buffer->maxsize) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    buffer->messages[buffer->count] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->count = atomic_fetch_add(&buffer->count, 1) % buffer->maxsize;
    printf("non_blocking_put() - Buffer count: %lu\n", buffer->count);

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "non_blocking_put() - Returning NULL msg\n");

    return msg;
}

msg_t *blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "blocking_get() - Null buffer found\n");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == 0) {
        pthread_cond_wait(&(buffer->empty), &(buffer->mutex));
    }

    long buffer_index = buffer->count - 1;
    if (buffer->messages == NULL || buffer->messages[buffer_index] == NULL) {
        printf("Null message at index: %lu\n", buffer_index);
        return BUFFER_ERROR;
    }

    msg_t *msg = buffer->messages[buffer_index];
    atomic_fetch_sub(&(buffer->count), 1);
    printf("blocking_put() - Buffer count: %lu\n", buffer_index);

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "non_blocking_get() - NULL msg");

    return msg;
}

msg_t *non_blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_get() - Null buffer found\n");

    long buffer_index = buffer->count - 1;
    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == 0 || buffer->messages[buffer_index] == NULL) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    msg_t *msg = buffer->messages[buffer_index];
    atomic_fetch_sub(&(buffer_index), 1);
    printf("non_blocking_put() - Buffer count: %lu\n", buffer_index);

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "non_blocking_get() - NULL msg");

    return msg;
}
