//
// Created by francesco_pio_montrano on 08/06/23.
//


#include "../headers/msg_t.h"

#include <string.h>
#include <stdlib.h>

/**/
msg_t *msg_init(void *content) {
    check(content != NULL, "msg_init() - Null content found...\n");

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
    if (msg != NULL) {
        free(msg);
    }
}

msg_t *msg_copy(msg_t *msg) {
    check(msg != NULL, "msg_copy() - Null msg found...\n");

    msg_t *new_msg = (msg_t *) malloc(sizeof(msg_t));

    new_msg->content = msg->content;

    return new_msg;
}
