#include <stdio.h>
#include "CuTest.h"

#define MONKEDORE_IMPLEMENTATION
#include "../monkedore.h"

CuSuite* MiscInstructionsGetSuite();
CuSuite* StackHandlingGetSuite();
CuSuite* MemoryHandlingGetSuite();
CuSuite* ArithmeticGetSuite();

void RunTests() {
    CuString* output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, MiscInstructionsGetSuite());
    CuSuiteAddSuite(suite, StackHandlingGetSuite());
    CuSuiteAddSuite(suite, MemoryHandlingGetSuite());
    CuSuiteAddSuite(suite, ArithmeticGetSuite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer); 
}

int main() {
    RunTests();
    return 0;
}
