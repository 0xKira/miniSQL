#include <iostream>
#include "RecordManager.h"
#include "BufferManager.h"
#include "API.h"
//#include "BpTree.h"
#include "Catalog.h"

const size_t blockSize = 4096;
const size_t blockNum = 256;
BufferManager bm(blockSize, blockNum);
RecordManager rm;

int main() {

}