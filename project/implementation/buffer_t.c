//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <stdio.h>
#include <stdlib.h>

msg_t *generate_put_msg(buffer_t *buffer, const msg_t *msg);

msg_t *generate_get_msg(buffer_t *buffer, long buffer_index);

buffer_t *buffer_init(unsigned long maxsize) {
    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));

    buffer->messages = malloc(sizeof(msg_t *) * buffer->maxsize);
    buffer->count = 0;
    buffer->maxsize = maxsize;

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->full), NULL);
    pthread_cond_init(&(buffer->empty), NULL);

    return buffer;
}

void buffer_destroy(buffer_t *buffer) {
    check(buffer != NULL, "buffer_destroy() - NULL buffer found\n");

    if (buffer->messages != NULL) {
        for (int i = 0; i < buffer->maxsize && buffer->messages[i] != BUFFER_ERROR; i++) {
            msg_destroy(buffer->messages[i]);
        }
        free(buffer->messages);
        buffer->messages = NULL;
    }

    pthread_mutex_destroy(&(buffer->mutex));
    pthread_cond_destroy(&(buffer->full));
    pthread_cond_destroy(&(buffer->empty));

    free(buffer);
    buffer = NULL;
}

msg_t *blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "blocking_put() - NULL buffer found\n");
    check(msg != BUFFER_ERROR, "blocking_put() - NULL msg found\n");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == buffer->maxsize) {
        pthread_cond_wait(&(buffer->full), &(buffer->mutex));
    }

    return generate_put_msg(buffer, msg);
}

msg_t *non_blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "non_blocking_put() - NULL buffer found\n");
    check(msg != BUFFER_ERROR, "non_blocking_put() - NULL msg found\n");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == buffer->maxsize) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    return generate_put_msg(buffer, msg);
}

msg_t *blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "blocking_get() - NULL buffer found\n");
    check(buffer->messages != NULL, "blocking_get() - NULL buffer messages found\n");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == 0) {
        pthread_cond_wait(&(buffer->empty), &(buffer->mutex));
    }

    long buffer_index = buffer->count - 1;
    if (buffer->messages == NULL || buffer->messages[buffer_index] == NULL) {
        printf("Null message at index: %lu\n", buffer_index);
        return BUFFER_ERROR;
    }

    return generate_get_msg(buffer, buffer_index);
}

msg_t *non_blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_get() - NULL buffer found\n");

    long buffer_index = buffer->count - 1;
    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == 0 || buffer->messages[buffer_index] == NULL) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    return generate_get_msg(buffer, buffer_index);
}

msg_t *generate_get_msg(buffer_t *buffer, long buffer_index) {
    msg_t *msg = buffer->messages[buffer_index];
    atomic_fetch_sub(&(buffer->count), 1);
    printf("get() - Buffer count: %lu\n", buffer_index);

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "get() - NULL msg");

    return msg;
}

msg_t *generate_put_msg(buffer_t *buffer, const msg_t *msg) {
    msg_t *msg_copy = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->messages[buffer->count] = msg_copy;
    buffer->count = atomic_fetch_add(&buffer->count, 1) % buffer->maxsize;
    printf("put() - Buffer count: %lu\n", buffer->count);

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));
    check(msg != BUFFER_ERROR, "put() - Returning NULL msg\n");

    return msg_copy;
}
