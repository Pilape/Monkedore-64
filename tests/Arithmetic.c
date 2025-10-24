#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestAdd(CuTest* tc) {
    // No carry
    {
        monkedore_Byte program[] = {
            0x02, 0x40, 0x00, 0x02, 0x20, 0x22, 0x0D, 0x01,
        };
        VM_INIT();

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, 0x4000+0x2022, VM_STACK_TOP(vm.data_stack));
        CuAssertIntEquals(tc, 0, vm.carry);
    }

    // Yes carry
    {
        monkedore_Byte program[] = {
            0x02, 0xff, 0xff, 0x02, 0x23, 0x24, 0x0D, 0x01,
        };
        VM_INIT();

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, (0xffff+0x2324) & 0xFFFF, VM_STACK_TOP(vm.data_stack));
        CuAssertIntEquals(tc, 1, vm.carry);
    }
}

CuSuite* ArithmeticGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestAdd);
    return suite;
}
