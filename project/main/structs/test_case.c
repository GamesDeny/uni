//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "test_case.h"
#include "utils.c"

/*
 * Funzione per la lettura dei test_case da CSV
*/
test_case **read_CSV() {
    FILE *file;
    char line[TEST_CASES];
    char *token;
    test_case **data = (test_case **) malloc(TEST_CASES * sizeof(test_case *));

    // Open the CSV file for reading
    file = fopen(CSV_PATH, FILE_MODE);
    check(file != NULL, "read_CSV() - Failed to open the file.");

    // Read the file line by line
    for (int row = 0; fgets(line, sizeof(line), file) != NULL && row < TEST_CASES; row++) {
        data[row] = (test_case *) malloc(sizeof(test_case));
        char line_copy[TEST_CASES];
        strcpy(line_copy, line);  // Create a copy of the line

        // Tokenize the copied line using the specified delimiter
        token = strtok(line_copy, CSV_DELIM);
        for (int column = 0; token != NULL && column < NO_OF_COLUMNS; column++) {
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

void prepare_argv(test_case **cases, int num_cases, int *argc, char ***argv) {
    *argc = num_cases * 3 + 1;  // Numero di parametri + 1 per il nome del programma
    *argv = (char **) malloc((*argc) * sizeof(char *));

    // Imposta il nome del programma come primo parametro
    (*argv)[0] = strdup(CSV_PATH);

    // Prepara i parametri per ogni test_case 
    for (int i = 0; i < num_cases; i++) {
        (*argv)[i * 3 + 1] = (char *) malloc(
                20 * sizeof(char));  // Assumendo una lunghezza massima di 20 caratteri per i parametri
        sprintf((*argv)[i * 3 + 1], "%d", cases[i]->no_consumers);

        (*argv)[i * 3 + 2] = (char *) malloc(20 * sizeof(char));
        sprintf((*argv)[i * 3 + 2], "%d", cases[i]->no_producers);

        (*argv)[i * 3 + 3] = (char *) malloc(20 * sizeof(char));
        sprintf((*argv)[i * 3 + 3], "%d", cases[i]->buffer_size);
    }

    // Ultimo elemento di argv deve essere NULL
    (*argv)[*argc - 1] = NULL;
}
