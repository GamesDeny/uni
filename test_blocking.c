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

    CU_add_test(suite, "Test Scenario 1", test_scenario_1);
    CU_add_test(suite, "Test Scenario 2", test_scenario_2);
    CU_add_test(suite, "Test Scenario 3", test_scenario_3);
    CU_add_test(suite, "Test Scenario 4", test_scenario_4);
    CU_add_test(suite, "Test Scenario 5", test_scenario_5);
    CU_add_test(suite, "Test Scenario 6", test_scenario_6);
    CU_add_test(suite, "Test Scenario 7", test_scenario_7);
    CU_add_test(suite, "Test Scenario 8", test_scenario_8);
    CU_add_test(suite, "Test Scenario 9", test_scenario_9);
    CU_add_test(suite, "Test Scenario 10", test_scenario_4);
    CU_add_test(suite, "Test Scenario 11", test_scenario_11);

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
    CU_ASSERT_EQUAL(buffer->count, 1);

    buffer_destroy(buffer);
}

void test_scenario_2() {
    buffer_t *buffer = buffer_init(1);
    CU_ASSERT_EQUAL(buffer->count, 0)

    buffer->messages[buffer->count] = msg_init("val");
    buffer->count++;
    CU_ASSERT_EQUAL(buffer->count, 1);

    pthread_t consumer_t;
    pthread_create(&consumer_t, NULL, blocking_consumer, buffer);

    pthread_join(consumer_t, NULL);
    CU_ASSERT_EQUAL(buffer->count, 0);

    buffer_destroy(buffer);
}

void test_scenario_3() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t;
    pthread_t consumer_t;

    pthread_create(&producer_t, NULL, blocking_producer, buffer);
    pthread_create(&consumer_t, NULL, blocking_consumer, buffer);

    pthread_join(consumer_t, NULL);
    pthread_join(producer_t, NULL);

    buffer_destroy(buffer);
}

void test_scenario_4() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t_1;
    pthread_t producer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(producer_t_2, NULL);

    buffer_destroy(buffer);
}

void test_scenario_5() {
    buffer_t *buffer = buffer_init(1);

    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);

    pthread_join(consumer_t_1, NULL);
    pthread_join(consumer_t_2, NULL);

    buffer_destroy(buffer);
}

void test_scenario_6() {
    buffer_t *buffer = buffer_init(2);

    pthread_t producer_t_1;
    pthread_t producer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(producer_t_2, NULL);

    buffer_destroy(buffer);
}

void test_scenario_7() {
    buffer_t *buffer = buffer_init(4);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t producer_t_3;
    pthread_t producer_t_4;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_3, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_4, NULL, blocking_producer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(producer_t_2, NULL);
    pthread_join(producer_t_3, NULL);
    pthread_join(producer_t_4, NULL);

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
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_3, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_4, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_5, NULL, blocking_producer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(producer_t_2, NULL);
    pthread_join(producer_t_3, NULL);
    pthread_join(producer_t_4, NULL);
    pthread_join(producer_t_5, NULL);

    buffer_destroy(buffer);
}

void test_scenario_9() {
    buffer_t *buffer = buffer_init(2);

    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);

    pthread_join(consumer_t_1, NULL);
    pthread_join(consumer_t_2, NULL);

    buffer_destroy(buffer);
}

void test_scenario_10() {
    buffer_t *buffer = buffer_init(1);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(consumer_t_1, NULL);
    pthread_join(producer_t_2, NULL);
    pthread_join(consumer_t_2, NULL);

    buffer_destroy(buffer);
}

void test_scenario_11() {
    buffer_t *buffer = buffer_init(2);

    pthread_t producer_t_1;
    pthread_t producer_t_2;
    pthread_t consumer_t_1;
    pthread_t consumer_t_2;

    pthread_create(&producer_t_1, NULL, blocking_producer, buffer);
    pthread_create(&producer_t_2, NULL, blocking_producer, buffer);
    pthread_create(&consumer_t_1, NULL, blocking_consumer, buffer);
    pthread_create(&consumer_t_2, NULL, blocking_consumer, buffer);

    pthread_join(producer_t_1, NULL);
    pthread_join(consumer_t_1, NULL);
    pthread_join(producer_t_2, NULL);
    pthread_join(consumer_t_2, NULL);

    buffer_destroy(buffer);
}
