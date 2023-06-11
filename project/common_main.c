//
// Created by francesco_pio_montrano on 08/11/23.
//

#include "headers/buffer_t.h"

#include "implementation/utils.c"
#include "implementation/buffer_t.c"
#include "implementation/msg_t.c"
#include "implementation/test_case.c"
#include "implementation/consumers.c"
#include "implementation/producers.c"

#define DEFAULT_BUFFER_SIZE 10
#define DEFAULT_NO_CONSUMERS 100
#define DEFAULT_NO_PRODUCERS 100

unsigned long no_of_consumers;
unsigned long no_of_producers;

void free_allocated_memory(int argc, char ***argv, test_case **cases);

int running_main(int argc, char **argv);

int main_with_args(int argc, char **argv);

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

    // Calling parameterized main_with_args
    for (int i = 0; i < TEST_CASES; i++) {
        running_main(argc, argv);
    }

    free_allocated_memory(argc, test_argv, cases);
    return 1;
}

void free_allocated_memory(int argc, char ***argv, test_case **cases) {
    for (int i = 0; i < sizeof argv; i++) {
        for (int j = 0; j < argc; j++) {
            free(argv[i][j]);
        }
        free(argv[i]);
    }
    free(argv);

    for (int i = 0; i < TEST_CASES; i++) {
        free(cases[i]);
    }
    free(cases);
}
