#include <iostream>
#include <cstdio>
#include <fstream>

#include "BufferManager.h"
#include "RecordManager.h"

size_t blockSize = 5;
BufferManager bm(5, 3);

void dump_hex(char *buff) {
    while (buff[0]) {
        printf("%d ", buff[0]);
        buff++;
    }
    putchar('\n');
}

int main() {
    char outBuffer[10];
    char inBuffer[10] = "11111";
    string a("aaaa");

    bm.readBlockData(a, 0, outBuffer);
    dump_hex(outBuffer);
    bm.readBlockData(a, 1, outBuffer);
    dump_hex(outBuffer);
    bm.writeBlockData(a, 1, inBuffer);
    return 0;
}