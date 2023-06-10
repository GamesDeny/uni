//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "structs/headers/buffer_t.h"

#include "structs/utils.c"
#include "structs/consumers.c"
#include "structs/producers.c"
#include "structs/test_case.c"
#include "structs/buffer_t.c"
#include "structs/msg_t.c"

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_NO_CONSUMERS 100
#define DEFAULT_NO_PRODUCERS 100
#define TEST_ARGV "test"

unsigned long no_of_consumers;
unsigned long no_of_producers;
int blocking;

void free_allocated_memory(int argc, char ***argv, test_case **cases);

int main();

int main_with_args(int argc, char **argv) {
    int is_test = argc > 1 && argv[1] != NULL ? 0 : 1;
    int test_cases = is_test ? 1 : TEST_CASES;

    test_case **cases = read_CSV();
    check(cases != NULL, "main() - No cases found");

    char ***test_argv = (char ***) malloc(test_cases * sizeof(char **));;
    for (size_t i = 0; i < sizeof cases; i++) {
        test_argv[i] = (char **) malloc(sizeof(cases) * sizeof(char **));
        for (size_t j = 0; j < sizeof(cases[i]); j++) {
            test_argv[i][j] = "-1";
        }
    }
    prepare_argv(cases, is_test, test_argv);
    blocking = 1;

    // Calling parameterized main_with_args
    // blocking_val:
    //  0 -> non_blocking producers and consumers
    //  1 -> blocking producers and consumers
    for (int i = 0; i < test_cases; i++) {
        no_of_consumers = (argv[1] != NULL) ? strtoul(argv[1], NULL, 10) : DEFAULT_NO_CONSUMERS;
        no_of_producers = (argv[2] != NULL) ? strtoul(argv[2], NULL, 10) : DEFAULT_NO_PRODUCERS;
        buffer_size = (argv[3] != NULL) ? strtoul(argv[3], NULL, 10) : DEFAULT_BUFFER_SIZE;
        main();
    }

    free_allocated_memory(4, test_argv, cases);
    return 1;
}

int main() {
    buffer_t *buffer = buffer_init(buffer_size);
    check(buffer != NULL, "main() - Error buffer null found");

    for (int cons = 0, prod = 0; cons < no_of_consumers && prod < no_of_producers; cons++, prod++) {
        printf("main() - step cons %d, step prod %d", cons, prod);

        pthread_t producer_t;
        pthread_t consumer_t;

        if (blocking) {
            // Blocking
            pthread_create(&producer_t, NULL, blocking_producer, buffer);
            pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
        } else {
            // Non blocking
            pthread_create(&producer_t, NULL, non_blocking_producer, buffer);
            pthread_create(&consumer_t, NULL, non_blocking_consumer, buffer);
        }
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