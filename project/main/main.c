//
// Created by francesco_pio_montrano on 08/06/23.
//

#include "structs/consumers.h"
#include "structs/producers.h"
#include "structs/test_case.h"
#include "structs/buffer_t.h"
#include "structs/utils.c"

#define DEFAULT_BUFFER_SIZE 1000
#define DEFAULT_NO_CONSUMERS 5
#define DEFAULT_NO_PRODUCERS 5

unsigned long no_of_consumers;
unsigned long no_of_producers;
int blocking;

int main_with_args(int blocking_val, int argc, char **argv);

void free_allocated_memory(int argc, char ***argv, test_case **cases);

int main() {
    test_case **cases = read_CSV();
    check(cases != NULL, "main() - No cases found");

    char ***argv = prepare_argv(cases);

    // Calling parameterized main_with_args
    // blocking_val:
    //  0 -> non_blocking producers and consumers
    //  1 -> blocking producers and consumers
    for (int i = 0; i < TEST_CASES; i++) {
        printf("Starting with cases: %d %d %d", *argv[i][1], *argv[i][2], *argv[i][3]);
        main_with_args(1, sizeof argv, argv[i]);
    }
    free_allocated_memory(TEST_CASES, argv, cases);

    return 0;
}

int main_with_args(int blocking_val, int argc, char **argv) {
    blocking = blocking_val > 0 ? 1 : 0;

    check(argc >= 0, "main() - Error with arg count, expected positive...");

    no_of_consumers = (argv[1] != NULL) ? strtoul(argv[1], NULL, 10) : DEFAULT_NO_CONSUMERS;
    no_of_producers = (argv[2] != NULL) ? strtoul(argv[2], NULL, 10) : DEFAULT_NO_PRODUCERS;
    buffer_size = (argv[3] != NULL) ? strtoul(argv[3], NULL, 10) : DEFAULT_BUFFER_SIZE;

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
    pthread_exit(NULL);
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

void *blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "%d", i);
        msg_t *message = msg_init(content);

        blocking_put(buffer, message);
        printf("Produced: %d\n", *(char *) message->content);
    }

    pthread_exit(NULL);
}

void *non_blocking_producer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "%d", i);
        msg_t *message = msg_init(content);

        non_blocking_put(buffer, message);
        printf("Produced: %d\n", *(char *) message->content);
    }

    pthread_exit(NULL);
}


void *blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        msg_t *message = blocking_get(buffer);

        printf("Consumed: %d\n", *(char *) message->content);
        msg_destroy(message);
    }

    pthread_exit(NULL);
}

void *non_blocking_consumer(void *arg) {
    buffer_t *buffer = (buffer_t *) arg;

    for (int i = 0; i < buffer_size; i++) {
        msg_t *message = non_blocking_get(buffer);

        printf("Consumed: %d\n", *(char *) message->content);
        msg_destroy(message);
    }

    pthread_exit(NULL);
}


buffer_t *buffer_init(unsigned int maxsize) {
    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));

    buffer->messages = (msg_t **) malloc(maxsize * sizeof(msg_t *));
    buffer->get_index = 0;
    buffer->put_index = 0;
    buffer->count = 0;
    buffer->maxsize = maxsize;

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->full), NULL);
    pthread_cond_init(&(buffer->empty), NULL);

    return buffer;
}

void buffer_destroy(buffer_t *buffer) {
    check(buffer != NULL, "buffer_destroy() - Null buffer found...");

    free(buffer->messages);

    pthread_mutex_destroy(&(buffer->mutex));

    pthread_cond_destroy(&(buffer->full));
    pthread_cond_destroy(&(buffer->empty));

    free(buffer);
}

msg_t *blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "blocking_put() - Null buffer found...");
    check(msg != NULL, "blocking_put() - Null msg found...");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == buffer->maxsize) {
        pthread_cond_wait(&(buffer->full), &(buffer->mutex));
    }

    buffer->messages[buffer->get_index] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->get_index = (buffer->get_index + 1) % buffer->maxsize;
    buffer->count++;

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;

}

msg_t *non_blocking_put(buffer_t *buffer, msg_t *msg) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");
    check(msg != NULL, "non_blocking_put() - Null msg found...");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == buffer->maxsize) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    buffer->messages[buffer->get_index] = (msg_t *) msg->msg_copy((struct msg_t *) msg);
    buffer->get_index = (buffer->get_index + 1) % buffer->maxsize;
    buffer->count++;

    pthread_cond_signal(&(buffer->empty));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

msg_t *blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");

    pthread_mutex_lock(&(buffer->mutex));
    while (buffer->count == 0) {
        pthread_cond_wait(&(buffer->empty), &(buffer->mutex));
    }

    msg_t *msg = buffer->messages[buffer->put_index];
    buffer->put_index = (buffer->put_index + 1) % buffer->maxsize;
    buffer->count--;

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

msg_t *non_blocking_get(buffer_t *buffer) {
    check(buffer != NULL, "non_blocking_put() - Null buffer found...");

    pthread_mutex_lock(&(buffer->mutex));
    if (buffer->count == 0) {
        pthread_mutex_unlock(&(buffer->mutex));
        return BUFFER_ERROR;
    }

    msg_t *msg = buffer->messages[buffer->put_index];
    buffer->put_index = (buffer->put_index + 1) % buffer->maxsize;
    buffer->count--;

    pthread_cond_signal(&(buffer->full));
    pthread_mutex_unlock(&(buffer->mutex));

    return msg;
}

msg_t *msg_init(void *content) {
    check(content != NULL, "msg_init() - Null content found...");

    msg_t *new_msg = (msg_t *) malloc(sizeof(msg_t));
    char *string = (char *) content;
    char *new_content = (char *) malloc(strlen(string) + 1); // +1 per \0 finale
    strcpy(new_content, string);

    new_msg->content = new_content;
    new_msg->msg_init = (struct msg_t *(*)(void *)) msg_init;
    new_msg->msg_destroy = (void (*)(struct msg_t *)) msg_destroy;
    new_msg->msg_copy = (struct msg_t *(*)(struct msg_t *)) msg_copy;

    return new_msg;
}

void msg_destroy(msg_t *msg) {
    check(msg != NULL, "msg_destroy() - Null msg found...");

    free(msg);
}

msg_t *msg_copy(msg_t *msg) {
    check(msg != NULL, "msg_copy() - Null msg found...");

    msg_t *new_msg = (msg_t *) malloc(sizeof(msg_t));

    new_msg->content = msg->content;

    return new_msg;
}

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

char ***prepare_argv(test_case **cases) {
    char ***argv_matrix = (char ***) malloc(TEST_CASES * sizeof(char **));

    for (int i = 0; i < TEST_CASES; i++) {
        char **argv = (char **) malloc((4) * sizeof(char *));
        //argv[0] = strdup(CSV_PATH);

        argv[0] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[0], "%d", cases[i]->no_consumers);

        argv[1] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[1], "%d", cases[i]->no_producers);

        argv[2] = (char *) malloc(20 * sizeof(char));
        sprintf(argv[2], "%d", cases[i]->buffer_size);

        argv_matrix[i] = argv;
    }

    return argv_matrix;
}