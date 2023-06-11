//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This functions checks a condition and prints the relative message (or default if message is empty or null)
 * If the condition is false, it exits the program
 * This function is used to check blocking conditions (like NULL msg after blocking_get or blocking_put)
 */
void check(int condition, char *err_message) {
    if (!condition) {
        if (strcmp(err_message, "") <= 0) {
            err_message = "Error... exiting program.\n";
        }

        printf("%s\n", err_message);

        exit(-1);
    }
}