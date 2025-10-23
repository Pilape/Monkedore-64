#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestPush(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x01, 0x69,
    };
    VM_INIT();
    monkedore_ExecuteVmCycle(&vm);

    CuAssertIntEquals(tc, 0x0169, vm.data_stack.data[vm.data_stack.ptr-1]);
}

void TestOverflow(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x67, 0x22,
    };
    VM_INIT();

    int has_overflown = 0;
    for (int i=0; i<258; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_ERROR_OVERFLOW) has_overflown = 1;
        vm.ip = 0;
    }
    CuAssertTrue(tc, has_overflown);
}

void TestDup(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x02, 0x67, 0x03, 0x01
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, VM_STACK_TOP(vm.data_stack), 0x0267);
    CuAssertIntEquals(tc, VM_STACK_ELEMENT(vm.data_stack, 1), 0x0267);
}

void TestOver(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x01, 0x55, 0x02, 0x59, 0x01, 0x04, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, VM_STACK_TOP(vm.data_stack), 0x0155);
    CuAssertIntEquals(tc, VM_STACK_ELEMENT(vm.data_stack, 1), 0x5901);
    CuAssertIntEquals(tc, VM_STACK_ELEMENT(vm.data_stack, 2), 0x0155);
}

void TestPop(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x00, 0x69, 0x05, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, vm.data_stack.ptr, 0);
}

void TestUnderflow(CuTest* tc) {
    monkedore_Byte program[] = {
        0x05,
    };
    VM_INIT();

    int has_underflown = 0;
    for (int i=0; i<15; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_ERROR_UNDERFLOW) has_underflown = 1;
        vm.ip = 0;
    }

    CuAssertTrue(tc, has_underflown);
}


CuSuite* StackHandlingGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestPush);
    SUITE_ADD_TEST(suite, TestOverflow);
    SUITE_ADD_TEST(suite, TestDup);
    SUITE_ADD_TEST(suite, TestOver);
    SUITE_ADD_TEST(suite, TestPop);
    SUITE_ADD_TEST(suite, TestUnderflow);
    return suite;
}
