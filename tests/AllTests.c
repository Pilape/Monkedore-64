#include <stdio.h>
#include "CuTest.h"

void RunTests(CuSuite* suite) {
    CuString* output = CuStringNew();

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer); 
}

int main() {
    CuSuite* suite = CuSuiteNew();
    RunTests(suite);
    return 0;
}
