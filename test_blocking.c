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

void add_message_to_buffer(buffer_t *buffer, int n);

void test_scenario_1();

void test_scenario_2();

void test_scenario_3();

void test_scenario_4();

void test_scenario_5();

void test_scenario_6();

void test_scenario_7();

void test_scenario_8();

void test_scenario_9();

void test_scenario_10();

void test_scenario_11();

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Main Test Suite", NULL, NULL);

    CU_add_test(suite, "Test Scenario 1=P:1, C:0, N:1", test_scenario_1);
    CU_add_test(suite, "Test Scenario 2=P:0, C:1, N:1", test_scenario_2);
    CU_add_test(suite, "Test Scenario 3=P:1, C:1, N:1", test_scenario_3);
    CU_add_test(suite, "Test Scenario 4=P:2, C:0, N:1", test_scenario_4);
    CU_add_test(suite, "Test Scenario 5=P:0, C:2, N:1", test_scenario_5);
    CU_add_test(suite, "Test Scenario 6=P:2, C:0, N:2", test_scenario_6);
    CU_add_test(suite, "Test Scenario 7=P:4, C:0, N:4", test_scenario_7);
    CU_add_test(suite, "Test Scenario 8=P:5, C:0, N:5", test_scenario_8);
    CU_add_test(suite, "Test Scenario 9=P:0, C:2, N:2", test_scenario_9);
    CU_add_test(suite, "Test Scenario 10=P:2, C:2, N:1", test_scenario_4);
    CU_add_test(suite, "Test Scenario 11=P:2, C:2, N:2", test_scenario_11);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();

    return CU_get_error();
}

void test_scenario_1() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t;
    pthread_create(&producer_t, NULL, blocking_producer, buffer);
    pthread_join(producer_t, NULL);

    CU_ASSERT_EQUAL(buffer->count, 1)

    buffer_destroy(buffer);
}

void test_scenario_2() {
    buffer_t *buffer = buffer_init(1);

    add_message_to_buffer(buffer, 1);
    CU_ASSERT_EQUAL(buffer->count, 1)
    CU_ASSERT_EQUAL(sizeof buffer->messages, 1)

    pthread_t consumer_t;
    pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_scenario_3() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t;
    pthread_t consumer_t;

    pthread_create(&producer_t, NULL, blocking_producer, buffer);
    pthread_join(producer_t, NULL);
    pthread_create(&consumer_t, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_scenario_4() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t_1;
    pthread_t producer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 2)

    buffer_destroy(buffer);
}

void test_scenario_5() {
    buffer_t *buffer = buffer_init(1);
    add_message_to_buffer(buffer, 2);
    CU_ASSERT_EQUAL(buffer->count, 2)
    CU_ASSERT_EQUAL(sizeof buffer->messages, 2)

    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_1, NULL);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_scenario_6() {
    buffer_t *buffer = buffer_init(2);

    pthread_t producer_t_1;
    pthread_t producer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 2)

    buffer_destroy(buffer);
}

void test_scenario_7() {
    buffer_t *buffer = buffer_init(4);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t producer_t_3;
    pthread_t producer_t_4;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);
    pthread_create(&producer_t_3, NULL, blocking_producer, buffer);
    pthread_join(producer_t_3, NULL);
    pthread_create(&producer_t_4, NULL, blocking_producer, buffer);
    pthread_join(producer_t_4, NULL);

    CU_ASSERT_EQUAL(buffer->count, 4)

    buffer_destroy(buffer);
}

void test_scenario_8() {
    buffer_t *buffer = buffer_init(5);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t producer_t_3;
    pthread_t producer_t_4;
    pthread_t producer_t_5;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);
    pthread_create(&producer_t_3, NULL, blocking_producer, buffer);
    pthread_join(producer_t_3, NULL);
    pthread_create(&producer_t_4, NULL, blocking_producer, buffer);
    pthread_join(producer_t_4, NULL);
    pthread_create(&producer_t_5, NULL, blocking_producer, buffer);
    pthread_join(producer_t_5, NULL);

    CU_ASSERT_EQUAL(buffer->count, 5)

    buffer_destroy(buffer);
}

void test_scenario_9() {
    buffer_t *buffer = buffer_init(2);
    add_message_to_buffer(buffer, 2);
    CU_ASSERT_EQUAL(buffer->count, 2)
    CU_ASSERT_EQUAL(sizeof buffer->messages, 2)

    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_1, NULL);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_scenario_10() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void test_scenario_11() {
    buffer_t *buffer = buffer_init(2);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_join(producer_t_1, NULL);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_join(producer_t_2, NULL);
    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_1, NULL);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);
    pthread_join(consumer_t_2, NULL);

    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer_destroy(buffer);
}

void add_message_to_buffer(buffer_t *buffer, int n) {
    for (int i = 0; i < n; i++) {
        void *content = malloc(sizeof(char *));
        snprintf(content, sizeof(content), "val %d", i);

        buffer->messages[buffer->count] = msg_init(content);
        buffer->count++;
    }
}