#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestShiftLeft(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x0F, 0x0F, 0x02, 0x00, 0x04, 0x11, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, 0x0F0F<<4, VM_STACK_TOP(vm.data_stack));
}

void TestShiftRight(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x0F, 0x0F, 0x02, 0x00, 0x0F, 0x12, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, 0x0F0F>>15, VM_STACK_TOP(vm.data_stack));
}

void TestbNand(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x25, 0x26, 0x02, 0x27, 0x28, 0x13, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, (monkedore_Word)~(0x2526 & 0x2728), VM_STACK_TOP(vm.data_stack));
}

CuSuite* BitwiseGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestShiftLeft);
    SUITE_ADD_TEST(suite, TestShiftRight);
    SUITE_ADD_TEST(suite, TestbNand);
    return suite;
}
