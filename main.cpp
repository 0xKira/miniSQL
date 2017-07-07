#include <iostream>
#include "RecordManager.h"
#include "BufferManager.h"
#include "API.h"
//#include "BpTree.h"
#include "Catalog.h"
#include "Interpreter.h"

const size_t blockSize = 40;
const size_t blockNum = 256;
BufferManager bm(blockSize, blockNum);
RecordManager rm;
Catalog cm;
Interpreter itp;
API api;
bool exitFlag = false;

int main() {
    while (true) {
        try {
            itp.EXEC();
        }
        catch (runtime_error &err) {
            cerr << err.what() << endl;
        }
        if (exitFlag)
            break;
    }

}
// create table aaa(name int unique,city char(8) unique,test float);
// insert into aaa values (1323,'wbbbaa34',22.555);
// insert into aaa values (8623,'wsdq1234',99.555);
// insert into aaa values (-1234,'0000aaaa',-9.88);
// insert into aaa values (-9999,'eeeeaaaa',100);
// insert into aaa values (200,'8888aaaa',1.123);
// delete from aaa where test<50;
// select * from aaa;
// select * from aaa where name2=1234 and name2>10000;
// select * from aaa where name2>12 and name2<10000;

