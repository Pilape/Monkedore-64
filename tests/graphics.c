#include <stdlib.h>
#include "CuTest.h"

#include "../monkedore.h"
#include "testvmsetup.h"

void TestDrawSprite(CuTest* tc) {
    monkedore_Byte program[] = { 0 };
    VM_INIT();
}

CuSuite* GraphicsGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestDrawSprite);
    return suite;
}
