#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestNand(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x00, 0x01, 0x02, 0x00, 0x01, 0x14, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, !(1 && 1), VM_STACK_TOP(vm.data_stack));
}

void TestEqual(CuTest* tc) {
    {
        monkedore_Byte program[] = {
            0x02, 0x22, 0x21, 0x02, 0x22, 0x21, 0x15, 0x01,
        };
        VM_INIT();

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, 0x2221 == 0x2221, VM_STACK_TOP(vm.data_stack));
    }
    {
        monkedore_Byte program[] = {
                0x02, 0x42, 0x29, 0x02, 0x22, 0x21, 0x15, 0x01,
        };
        VM_INIT();

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, 0x4229 == 0x2221, VM_STACK_TOP(vm.data_stack));
    }
}

CuSuite* BooleanGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestNand);
    SUITE_ADD_TEST(suite, TestEqual);
    return suite;
}
