//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "common_main.c"

int main() {
    for (int test = 0; test < 2; test++) {
        if (test) {
            main_with_args(1, (char **) NULL);
        } else {
            char **argv = (char **) malloc((4) * sizeof(char *));
            argv[0] = strdup(CSV_PATH);

            argv[1] = (char *) malloc(20 * sizeof(char));
            sprintf(argv[1], "%d", 100);

            argv[2] = (char *) malloc(20 * sizeof(char));
            sprintf(argv[2], "%d", 100);

            argv[3] = (char *) malloc(20 * sizeof(char));
            sprintf(argv[3], "%d", 10);

            running_main(4, (char **) argv);
        }
    }
}

int running_main(int argc, char **argv) {
    if (argc > 1 && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL) {
        no_of_consumers = strtoul(argv[1], NULL, 10);
        no_of_producers = strtoul(argv[2], NULL, 10);
        buffer_size = strtoul(argv[3], NULL, 10);
    } else {
        no_of_consumers = DEFAULT_NO_CONSUMERS;
        no_of_producers = DEFAULT_NO_PRODUCERS;
        buffer_size = DEFAULT_BUFFER_SIZE;
    }

    buffer_t *buffer = buffer_init(buffer_size);
    check(buffer != NULL, "main() - Error buffer null found\n");

    for (unsigned long cons = 0, prod = 0; cons < no_of_consumers && prod < no_of_producers; cons++, prod++) {
        printf("main() - step cons %lu, step prod %lu\n", cons, prod);

        pthread_t producer_t;
        pthread_t consumer_t;

        pthread_create(&producer_t, NULL, blocking_producer, buffer);
        pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
    }

    buffer_destroy(buffer);
    return 0;
}
