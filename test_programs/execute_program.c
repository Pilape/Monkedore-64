#define MONKEDORE_IMPLEMENTATION
#include "../monkedore.h"

#include <stdio.h>
#include <stdlib.h>

int GetByteCodeLength(char* path) {
    FILE* fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Could not open file\n");
        exit(-1);
    }

    fseek(fp, 0, SEEK_END);

    int length = ftell(fp)/3;
    
    fclose(fp);

    return length;
}

void LoadByteCode(monkedore_Byte* dest, char* path) {
    FILE* fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Could not open file\n");
        exit(-1);
    }

    int i = 0;
    int temp = 0;
    while (fscanf(fp, "%02x ", &temp) == 2) {
        dest[i] = temp;
        if (i % 16 == 15) fscanf(fp, "\n");
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No file specified\n");
        return -1;
    }

    monkedore_Vm creature_of_steel = { 0 };
    monkedore_InitVM(&creature_of_steel);

    int length = GetByteCodeLength(argv[1]);
    monkedore_Byte source[length];
    LoadByteCode(source, argv[1]);
    for (int i=0; i<length; i++) {
        printf("%02x ", source[i]);
        if (i % 16 == 15) printf("\n");
    }

    monkedore_LoadProgram(&creature_of_steel, source, length);
    monkedore_ReturnStatus status = monkedore_SUCCESS;

    while(status == monkedore_SUCCESS) {
        status = monkedore_ExecuteVmCycle(&creature_of_steel);
        printf("%d | %d\n", creature_of_steel.ip, creature_of_steel.data_stack.ptr);
    }

    switch (status) {
        case monkedore_ERROR:
            printf("An error has occured\n");
            return -1;
        case monkedore_ERROR_OVERFLOW:
            printf("Stack overflow\n");
            return -1;
        case monkedore_ERROR_UNDERFLOW:
            printf("Stack underflow\n");
            return -1; 
    }

    for (int i=creature_of_steel.data_stack.ptr; i>=0; i--) {
        printf("%d\n", creature_of_steel.data_stack.data[i]);
    }

    return 0;
}
