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

        vm.carry = 1;

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

        vm.carry = 0;

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, (0xffff+0x2324) & 0xFFFF, VM_STACK_TOP(vm.data_stack));
        CuAssertIntEquals(tc, 1, vm.carry);
    }
}

void TestSub(CuTest* tc) {
     // No carry
    {
        monkedore_Byte program[] = {
            0x02, 0x40, 0x00, 0x02, 0x20, 0x22, 0x0E, 0x01,
        };
        VM_INIT();

        vm.carry = 1;

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, 0x4000-0x2022, VM_STACK_TOP(vm.data_stack));
        CuAssertIntEquals(tc, 0, vm.carry);
    }

    // Yes carry
    {
        monkedore_Byte program[] = {
            0x02, 0x25, 0x24, 0x02, 0xFF, 0xFF, 0x0E, 0x01,
        };
        VM_INIT();

        vm.carry = 0;

        while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

        CuAssertIntEquals(tc, (0x2524-0xFFFF) & 0xFFFF, VM_STACK_TOP(vm.data_stack));
        CuAssertIntEquals(tc, 1, vm.carry);
    }
   
}

CuSuite* ArithmeticGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestAdd);
    SUITE_ADD_TEST(suite, TestSub);
    return suite;
}
