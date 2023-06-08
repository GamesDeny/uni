//
// Created by francesco_pio_montrano on 08/06/23.
//

#ifndef UNI_TESTCASE_H
#define UNI_TESTCASE_H

#define CSV_PATH "test_cases/input.csv"
#define FILE_MODE "r"
#define CSV_DELIM ";"
#define TEST_CASES 13
#define NO_OF_COLUMNS 3

typedef struct {
    int no_consumers;
    int no_producers;
    int buffer_size;
} test_case;

char ***prepare_argv(test_case **cases);

test_case **read_CSV();

#endif //UNI_TESTCASE_H
