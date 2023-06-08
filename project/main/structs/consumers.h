//
// Created by francesco_pio_montrano on 08/06/23.
//

#ifndef UNI_CONSUMERS_H
#define UNI_CONSUMERS_H

unsigned long buffer_size;

void *blocking_consumer(void *arg);

void *non_blocking_consumer(void *arg);

#endif //UNI_CONSUMERS_H
