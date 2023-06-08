//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "test_case.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Funzione per la lettura dei test_case da CSV
*/
test_case **read_CSV() {
    FILE *file;
    char line[TEST_CASES];
    char *token;
    test_case **data = (test_case **) malloc(TEST_CASES * sizeof(test_case *));

    for (int row = 0; row < TEST_CASES; row++) {
        data[row] = (test_case *) malloc(sizeof(test_case));
        data[row]->no_consumers = -1;
        data[row]->no_producers = -1;
        data[row]->buffer_size = -1;
    }

    // Open the CSV file for reading
    file = fopen(CSV_PATH, FILE_MODE);
    check(file != NULL, "read_CSV() - Failed to open the file.");

    // Read the file line by line
    for (int row = 0; fgets(line, sizeof(line), file) != NULL && row < TEST_CASES; row++) {
        char line_copy[TEST_CASES];
        strcpy(line_copy, line);  // Create a copy of the line

        // Tokenize the copied line using the specified delimiter
        token = strtok(line_copy, CSV_DELIM);
        for (int column = 0; token != NULL && strcmp(token, "\n") > 0 && column < NO_OF_COLUMNS; column++) {
            // Convert token to integer and store in the data structure
            int value = atoi(token);
            switch (column) {
                case 0:
                    data[row]->no_consumers = value;
                    break;
                case 1:
                    data[row]->no_producers = value;
                    break;
                case 2:
                    data[row]->buffer_size = value;
                    break;
                default:
                    printf("readCSV() - Error with column:%d", column);
                    exit(-1);
            }
            token = strtok(NULL, CSV_DELIM);
        }
    }

    fclose(file);

    return data;
}

char ***prepare_argv(test_case **cases) {
    char ***argv_matrix = (char ***) malloc(TEST_CASES * sizeof(char **));

    for (int i = 0; i < TEST_CASES; i++) {
        char **argv = (char **) malloc((4) * sizeof(char *));
        argv[0] = strdup(CSV_PATH);

        argv[1] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[1], "%d", cases[i]->no_consumers);

        argv[2] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[2], "%d", cases[i]->no_producers);

        argv[3] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[3], "%d", cases[i]->buffer_size);

        argv_matrix[i] = argv;
    }

    return argv_matrix;
}