//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "../headers/consumers.h"
#include <stdio.h>
#include <stdlib.h>

void *blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    msg_t *message = blocking_get(buffer);

    check(message != NULL, "message null\n");
    printf("Consumed: %lu\n", buffer->get_index);
    msg_destroy(message);


    pthread_exit(NULL);
}

void *non_blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    msg_t *message = non_blocking_get(buffer);

    printf("Consumed: %s\n", message->content);
    msg_destroy(message);


    pthread_exit(NULL);
}
