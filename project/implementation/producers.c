//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "../headers/producers.h"
#include <stdio.h>
#include <stdlib.h>

void *blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    void *content = malloc(sizeof(char *));
    snprintf(content, sizeof(content), "%d", rand());
    msg_t *msg = msg_init(content);

    msg = blocking_put(buffer, msg);
    printf("Produced: %s\n", msg->content);
    printf("main() - buffer_count: %lu\n", buffer->count);

    pthread_exit(NULL);
}

void *non_blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    void *content = malloc(sizeof(char *));
    snprintf(content, sizeof(content), "%d", rand());
    msg_t *msg = msg_init(content);
    check(msg != NULL && msg != BUFFER_ERROR, "non_blocking_producer() - NULL msg created\n");

    msg = non_blocking_put(buffer, msg);
    check(msg != NULL && msg != BUFFER_ERROR, "non_blocking_producer() - NULL msg put\n");

    printf("Produced: %s\n", msg->content);
    printf("main() - buffer_count: %lu\n", buffer->count);

    //pthread_exit(NULL);
}
