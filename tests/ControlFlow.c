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

void TestJumpIfZero(CuTest* tc) {
    // Jump
    {
        monkedore_Byte program[] = {
            0x02, 0x00, 0x00, 0x19, 0x55, 0xFF,
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 0x55FF, vm.ip);
    }
    // No jump
    {
        monkedore_Byte program[] = {
            0x02, 0x0F, 0x21, 0x19, 0xFF, 0x55,
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 6, vm.ip);
    }

}

void TestJumpIfNotZero(CuTest* tc) {
    // No jump
    {
        monkedore_Byte program[] = {
            0x02, 0x00, 0x00, 0x1A, 0x05, 0x11
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 6, vm.ip);
    }
    // jump
    {
        monkedore_Byte program[] = {
            0x02, 0x0F, 0x21, 0x1A, 0x25, 0x11,
        };
        VM_INIT();

        for (int i=0; i<2; i++) { monkedore_ExecuteVmCycle(&vm); }

        CuAssertIntEquals(tc, 0x2511, vm.ip);
    }

}

void TestCall(CuTest* tc) {
    monkedore_Byte program[] = {
        0x00, 0x00, 0x1B, 0x25, 0x26,
    };

    VM_INIT();

    for (int i=0; i<3; i++) { monkedore_ExecuteVmCycle(&vm); }

    CuAssertIntEquals(tc, 0x2526, vm.ip);
    CuAssertIntEquals(tc, 5, VM_STACK_TOP(vm.return_stack));
}

void TestCallOverflow(CuTest* tc) {
    monkedore_Byte program[] = {
        0x1B, 0x00, 0x00,
    };
    VM_INIT();

    int has_overflown = 0;
    for (int i=0; i<260; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_ERROR_OVERFLOW) { has_overflown = 1; break; }
    }

    CuAssertTrue(tc, has_overflown);
}

void TestReturn(CuTest* tc) {
    monkedore_Byte program[] = {
        0x1C,
    };
    VM_INIT();
    
    vm.return_stack.data[vm.return_stack.ptr] = 0xF055;
    vm.return_stack.ptr++;

    monkedore_ExecuteVmCycle(&vm);

    CuAssertIntEquals(tc, 0xF055, vm.ip);
}

void TestReturnUnderflow(CuTest* tc) {
    monkedore_Byte program[] = {
        0x1C,
    };
    VM_INIT();

    int has_underflown = 0;
    for (int i=0; i<260; i++) {
        if (monkedore_ExecuteVmCycle(&vm) == monkedore_ERROR_UNDERFLOW) { has_underflown = 1; break; }
    }

    CuAssertTrue(tc, has_underflown);

}

CuSuite* ControlFlowGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestJump);
    SUITE_ADD_TEST(suite, TestJumpIfZero);
    SUITE_ADD_TEST(suite, TestJumpIfNotZero);
    SUITE_ADD_TEST(suite, TestCall);
    SUITE_ADD_TEST(suite, TestCallOverflow);
    SUITE_ADD_TEST(suite, TestReturn);
    SUITE_ADD_TEST(suite, TestReturnUnderflow);
    return suite;
}
