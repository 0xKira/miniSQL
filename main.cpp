#include <iostream>
#include "RecordManager.h"
#include "BufferManager.h"
#include "API.h"
//#include "BpTree.h"
#include "Catalog.h"
#include "Interpreter.h"

const size_t blockSize = 4096;
const size_t blockNum = 256;
BufferManager bm(blockSize, blockNum);
RecordManager rm;
Catalog cm;
Interpreter itp;

int main() {
    Tuple t;
    t.data.push_back(new DataI(100));
    t.data.push_back(new DataF(1.234));
    t.data.push_back(new DataS("Kira!"));
    rm.insertIntoTable(cm.getTable("aaaa"), t);
}