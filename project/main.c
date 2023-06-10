//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "headers/buffer_t.h"

#include "implementation/utils.c"
#include "implementation/consumers.c"
#include "implementation/producers.c"
#include "implementation/test_case.c"
#include "implementation/buffer_t.c"
#include "implementation/msg_t.c"

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_NO_CONSUMERS 100
#define DEFAULT_NO_PRODUCERS 100

unsigned long no_of_consumers;
unsigned long no_of_producers;
int blocking;

void free_allocated_memory(int argc, char ***argv, test_case **cases);

int running_main(int argc, char **argv);

int main_with_args(int argc, char **argv);

int main() {
    int test = 1;
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

int main_with_args(int argc, char **argv) {
    test_case **cases = read_CSV();
    check(cases != NULL, "main() - No cases found\n");

    char ***test_argv = (char ***) malloc(TEST_CASES * sizeof(char **));
    for (size_t i = 0; i < sizeof cases; i++) {
        test_argv[i] = (char **) malloc(sizeof(cases) * sizeof(char **));
        for (size_t j = 0; j < sizeof(cases[i]); j++) {
            test_argv[i][j] = 0;
        }
    }
    prepare_argv(cases, test_argv);
    blocking = 1;

    // Calling parameterized main_with_args
    // blocking_val:
    //  0 -> non_blocking producers and consumers
    //  1 -> blocking producers and consumers
    for (int i = 0; i < TEST_CASES; i++) {
        running_main(argc, argv);
    }

    free_allocated_memory(4, test_argv, cases);
    return 1;
}

int running_main(int argc, char **argv) {
    buffer_t *buffer = buffer_init(buffer_size);
    check(buffer != NULL, "main() - Error buffer null found\n");

    if (argc > 1 && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL) {
        no_of_consumers = strtoul(argv[1], NULL, 10);
        no_of_producers = strtoul(argv[2], NULL, 10);
        buffer_size = strtoul(argv[3], NULL, 10);
    } else {
        no_of_consumers = DEFAULT_NO_CONSUMERS;
        no_of_producers = DEFAULT_NO_PRODUCERS;
        buffer_size = DEFAULT_BUFFER_SIZE;
    }

    for (unsigned long cons = 0, prod = 0; cons < no_of_consumers && prod < no_of_producers; cons++, prod++) {
        printf("main() - step cons %lu, step prod %lu\n", cons, prod);

        pthread_t producer_t;
        pthread_t consumer_t;

        if (blocking) {
            pthread_create(&producer_t, NULL, blocking_producer, buffer);
            pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
        } else {
            pthread_create(&producer_t, NULL, non_blocking_producer, buffer);
            pthread_create(&consumer_t, NULL, non_blocking_consumer, buffer);
        }

        printf("main() - buffer_count: %lu\n", buffer->count);
    }

    buffer_destroy(buffer);
    return 0;
}

void free_allocated_memory(int argc, char ***argv, test_case **cases) {
    for (int i = 0, j = 0; i < argc && j < sizeof argv; i++, j++) {
        free(argv[j][i]);
    }
    free(argv);

    for (int i = 0; i < TEST_CASES; i++) {
        free(cases[i]);
    }
    free(cases);
}
