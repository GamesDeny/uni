//
// Created by francesco_pio_montrano on 08/06/23.
//

#include <CUnit/CUnit.h>

int main() {
    CU_cleanup_registry();

    CU_run_all_tests();

    return 0;
}