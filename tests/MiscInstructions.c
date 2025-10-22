#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

// VERY IMPORTANT. NOP has to do NOTHING. Or we will all die
void NopDoesNothing(CuTest* tc) {
    monkedore_Byte program[] = { 0 };
    VM_INIT();

    CuAssertIntEquals(tc, monkedore_ExecuteVmCycle(&vm), monkedore_SUCCESS);
    CuAssertIntEquals(tc, vm.ip, 1);

    CuAssertIntEquals(tc, vm.data_stack.ptr, 0);
    CuAssertIntEquals(tc, vm.data_stack.data[vm.data_stack.ptr-1], 0);

    CuAssertIntEquals(tc, vm.return_stack.ptr, 0);
    CuAssertIntEquals(tc, vm.return_stack.data[vm.data_stack.ptr-1], 0);
}

void TestHalt(CuTest* tc) {
    monkedore_Byte program[] = {
        0, 0, 0, 0, 0x01,
    };
    VM_INIT();

    int has_halted = 0;
    for (int i=0; i<15; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_HALTED) has_halted = 1;
    }
    CuAssertTrue(tc, has_halted);
}

CuSuite* MiscInstructionsGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, NopDoesNothing);
    SUITE_ADD_TEST(suite, TestHalt);
    return suite;
}
