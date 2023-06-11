//
// Created by francesco_pio_montrano on 08/06/23.
//


#include "../headers/msg_t.h"

#include <string.h>
#include <stdlib.h>

/**/
msg_t *msg_init(void *content) {
    check(content != NULL, "msg_init() - Null content found\n");

    //viene creata una copia "privata" della stringa
    msg_t *new_msg = (msg_t *) malloc(sizeof(msg_t));
    char *string = (char *) content;
    check(string != NULL, "msg_init() - NULL content found into string\n");

    char *new_content = (char *) malloc(strlen(string) + 1); // +1 per \0 finale
    check(new_content != NULL, "msg_init() - NULL content found into string\n");
    strcpy(new_content, string);

    new_msg->content = new_content;
    new_msg->msg_init = (struct msg_t *(*)(void *)) msg_init;
    new_msg->msg_destroy = (void (*)(struct msg_t *)) msg_destroy;
    new_msg->msg_copy = (struct msg_t *(*)(struct msg_t *)) msg_copy;

    return new_msg;
}

void msg_destroy(msg_t *msg) {
    if (msg != BUFFER_ERROR) {
        if (msg->content != NULL) {
            free(msg->content); // free copia privata
        }
        free(msg);          // free struct
    }
}

msg_t *msg_copy(msg_t *msg) {
    check(msg != BUFFER_ERROR, "msg_copy() - Null msg found\n");

    return (msg_t *) msg->msg_init(msg->content);
}
