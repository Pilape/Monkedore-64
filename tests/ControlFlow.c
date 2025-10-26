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

CuSuite* ControlFlowGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestJump);
    return suite;
}
