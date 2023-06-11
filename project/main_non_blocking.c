//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "common_main.c"

int main() {
    buffer_t *buffer = buffer_init(DEFAULT_BUFFER_SIZE);
    check(buffer != NULL, "main() - Error buffer null found\n");

    for (unsigned long cons = 0, prod = 0; cons < DEFAULT_NO_CONSUMERS && prod < DEFAULT_NO_PRODUCERS; cons++, prod++) {
        printf("main() - step cons %lu, step prod %lu\n", cons, prod);

        pthread_t producer_t;
        pthread_t consumer_t;

        pthread_create(&producer_t, NULL, non_blocking_producer, buffer);
        pthread_create(&consumer_t, NULL, non_blocking_consumer, buffer);
    }

    buffer_destroy(buffer);
    return 0;
}
