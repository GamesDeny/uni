//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "headers/producers.h"
#include <stdio.h>
#include <stdlib.h>

void *blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "%d", i);
        msg_t *message = msg_init(content);

        blocking_put(buffer, message);
        printf("Produced: %s\n", message->content);
    }

    pthread_exit(NULL);
}

void *non_blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "%d", i);
        msg_t *message = msg_init(content);

        non_blocking_put(buffer, message);
        printf("Produced: %s\n", message->content);
    }

    pthread_exit(NULL);
}
