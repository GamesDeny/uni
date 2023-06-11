//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "../headers/producers.h"
#include <stdio.h>
#include <stdlib.h>

void *blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;
    check(buffer != NULL, "non_blocking_producer() - NULL buffer found\n");

    void *content = malloc(sizeof(char *));
    snprintf(content, sizeof(content), "%d", rand());
    msg_t *msg = msg_init(content);

    msg = blocking_put(buffer, msg);
    if (msg != BUFFER_ERROR && msg->content != NULL) {
        printf("Produced: %s\n", msg->content);
    }

    printf("blocking_producer() - buffer_count: %lu\n", buffer->count);
    pthread_exit(NULL);
}

void *non_blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;
    check(buffer != NULL, "non_blocking_producer() - NULL buffer found\n");

    void *content = malloc(sizeof(char *));
    snprintf(content, sizeof(content), "%d", rand());
    msg_t *msg = msg_init(content);
    check(msg != BUFFER_ERROR, "non_blocking_producer() - NULL msg created\n");

    non_blocking_put(buffer, msg);

    if (msg != BUFFER_ERROR && msg->content != NULL) {
        printf("Produced: %s\n", msg->content);
    }
    printf("non_blocking_producer() - buffer_count: %lu\n", buffer->count);
    pthread_exit(NULL);
}
