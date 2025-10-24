#include <stdlib.h>
#include <stdio.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestLoad(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x04, 0xFF, 0x09, 0x01,
    };
    VM_INIT();

    vm.ram[0x04FF] = 0x69;
    vm.ram[0x0500] = 0x67;

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, 0x6967, VM_STACK_TOP(vm.data_stack));
    CuAssertIntEquals(tc, 1, vm.data_stack.ptr);
}

void TestLoadEdge(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0xFF, 0xFF, 0x09, 0x01,
    };
    VM_INIT();

    vm.ram[0xFFFF] = 0x11;

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, 0x1102, VM_STACK_TOP(vm.data_stack));
}

void TestStore(CuTest* tc) {
    monkedore_Byte program[] = {
        0x02, 0x67, 0x76, 0x02, 0x69, 0x96, 0x0A, 0x01,
    };
    VM_INIT();

    while (monkedore_ExecuteVmCycle(&vm) != monkedore_HALTED) { };

    CuAssertIntEquals(tc, 0x67, vm.ram[0x6996]);
    CuAssertIntEquals(tc, 0x76, vm.ram[0x6997]);
    CuAssertIntEquals(tc, 0, vm.data_stack.ptr);
}

CuSuite* MemoryHandlingGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestLoad);
    SUITE_ADD_TEST(suite, TestLoadEdge);
    SUITE_ADD_TEST(suite, TestStore);
    return suite;
}
