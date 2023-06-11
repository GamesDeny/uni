//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#include "project/headers/buffer_t.h"
#include "project/implementation/utils.c"
#include "project/implementation/buffer_t.c"
#include "project/implementation/msg_t.c"
#include "project/implementation/producers.c"
#include "project/implementation/consumers.c"

void test_buffer_init();

void test_buffer_put_and_get();

void test_buffer_destroy();

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Main Test Suite", NULL, NULL);

    CU_add_test(suite, "Test buffer_init", test_buffer_init);
    CU_add_test(suite, "Test buffer_destroy", test_buffer_destroy);
    CU_add_test(suite, "Test buffer_put_and_get", test_buffer_put_and_get);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}

void test_buffer_put_and_get() {
    buffer_t *buffer = buffer_init(100);

    pthread_t producer_t;
    pthread_create(&producer_t, NULL, non_blocking_producer, buffer);
    pthread_join(producer_t, NULL);
    CU_ASSERT_EQUAL(buffer->count, 1)

    pthread_t consumer_t;
    pthread_create(&consumer_t, NULL, non_blocking_consumer, buffer);
    pthread_join(consumer_t, NULL);
    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_buffer_destroy() {
    buffer_t *buffer = buffer_init(100);

    CU_ASSERT_NOT_EQUAL(buffer, NULL)
    CU_ASSERT_EQUAL(buffer->maxsize, 100)

    buffer_destroy(buffer);
    CU_ASSERT_EQUAL(buffer, NULL)
}

void test_buffer_init() {
    buffer_t *buffer = buffer_init(100);

    CU_ASSERT_PTR_NOT_NULL(buffer)
    CU_ASSERT_EQUAL(buffer->count, 0)
    CU_ASSERT_EQUAL(buffer->maxsize, 100)
    CU_ASSERT_NOT_EQUAL(&(buffer->mutex), NULL)

    buffer_destroy(buffer);
}
