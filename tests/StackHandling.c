#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"

void TestPush(CuTest* tc) {
    monkedore_Vm vm;
    
    monkedore_Byte program[] = {
            0x01, 0x69,
    };
    monkedore_LoadProgram(&vm, program, 2);

    // Does the instruction work
    monkedore_ExecuteVmCycle(&vm);

    CuAssertIntEquals(tc, 0x69, vm.data_stack.data[vm.data_stack.ptr-1]);

    // Overflow
    int has_overflown = 0;
    for (int i=0; i<258; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_ERROR_OVERFLOW) has_overflown = 1;
    }
    CuAssertTrue(tc, has_overflown);

}

CuSuite* StackHandlingGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestPush);
    return suite;
}
