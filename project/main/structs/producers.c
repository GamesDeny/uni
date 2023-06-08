//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "producers.h"
#include "buffer_t.h"
#include "utils.c"

void *blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "%d", i);
        msg_t *message = msg_init(content);

        blocking_put(buffer, message);
        printf("Produced: %d\n", *(char *) message->content);
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
        printf("Produced: %d\n", *(char *) message->content);
    }

    pthread_exit(NULL);
}