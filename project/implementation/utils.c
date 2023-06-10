//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check(int condition, char *err_message) {
    if (!condition) {
        if (strcmp(err_message, "") <= 0) {
            err_message = "Error... exiting program.\n";
        }

        printf("%s\n", err_message);

        exit(-1);
    }
}