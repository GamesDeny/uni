//
// Created by francesco_pio_montrano on 08/06/23.
//
#include "headers/buffer_t.h"

#include "implementation/utils.c"
#include "implementation/buffer_t.c"
#include "implementation/msg_t.c"
#include "implementation/consumers.c"
#include "implementation/producers.c"

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_NO_CONSUMERS 100
#define DEFAULT_NO_PRODUCERS 100

int main() {
    buffer_t *buffer = buffer_init(DEFAULT_BUFFER_SIZE);
    check(buffer != NULL, "main() - Error buffer null found\n");

    for (unsigned long cons = 0, prod = 0; cons < DEFAULT_NO_CONSUMERS && prod < DEFAULT_NO_PRODUCERS; cons++, prod++) {
        printf("main() - step cons %lu, step prod %lu\n", cons, prod);

        pthread_t producer_t;
        pthread_t consumer_t;

        pthread_create(&producer_t, NULL, blocking_producer, buffer);
        pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
    }

    buffer_destroy(buffer);
    return 0;
}
