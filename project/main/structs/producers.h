//
// Created by francesco_pio_montrano on 08/06/23.
//

#ifndef UNI_PRODUCERS_H
#define UNI_PRODUCERS_H

unsigned long buffer_size;

void *blocking_producer(void *arg);

void *non_blocking_producer(void *arg);

#endif //UNI_PRODUCERS_H
