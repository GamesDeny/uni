//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "../headers/consumers.h"
#include <stdio.h>
#include <stdlib.h>

void *blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    msg_t *msg = blocking_get(buffer);
    check(msg != NULL && msg != BUFFER_ERROR, "blocking_consumer() - msg null\n");

    msg_destroy(msg);
    printf("Consumed: %lu\n", buffer->count+1);
    printf("blocking_consumer() - buffer_count: %lu\n", buffer->count);

    pthread_exit(NULL);
}

void *non_blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;
    check(buffer != NULL, "non_blocking_consumer() - NULL buffer found\n");

    msg_t *msg = non_blocking_get(buffer);

    if (msg != NULL && msg != BUFFER_ERROR) {
        if (msg->content != NULL) {
            printf("non_blocking_consumer() - Consumed: %s\n", msg->content);
        } else {
            printf("non_blocking_consumer() - NULL content pointer\n");
        }
        msg_destroy(msg);
    } else {
        printf("non_blocking_consumer() - NULL msg pointer\n");
    }

    printf("non_blocking_consumer() - buffer_count: %lu\n", buffer->count);

//    pthread_exit(NULL);
}
