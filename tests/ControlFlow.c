#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestJump(CuTest* tc) {
    monkedore_Byte program[] = {
        0x18, 0xFF, 0x05,
    };
    VM_INIT();

    monkedore_ExecuteVmCycle(&vm);

    CuAssertIntEquals(tc, 0xFF05, vm.ip);
}

void TestBranch(CuTest* tc) {
    {
        monkedore_Byte program[] = {
            0x19, 25,
        };
        VM_INIT();

        monkedore_ExecuteVmCycle(&vm);

        CuAssertIntEquals(tc, 25+2, vm.ip);
    }
    {
        monkedore_Byte program[] = {
            0x19, -25,
        };
        VM_INIT();

        monkedore_ExecuteVmCycle(&vm);

        CuAssertIntEquals(tc, (monkedore_Word)(2 - 25 & 0xFFFF), vm.ip);
    }
}

void TestBranchIfZero(CuTest* tc) {
    // Jump
    {
        monkedore_Byte program[] = {
            0x02, 0x00, 0x00, 0x1A, 25,
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 25+5, vm.ip);
    }
    // No jump
    {
        monkedore_Byte program[] = {
            0x02, 0x0F, 0x21, 0x1A, -25,
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 5, vm.ip);
    }

}

CuSuite* ControlFlowGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestJump);
    SUITE_ADD_TEST(suite, TestBranch);
    SUITE_ADD_TEST(suite, TestBranchIfZero);
    return suite;
}
