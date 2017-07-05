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
API api;
bool exitFlag = false;

int main() {
    vector<Attribute> attrs;
    attrs.push_back(Attribute("id", 0, true, true));
    attrs.push_back(Attribute("fuck", -1, false, false));
    attrs.push_back(Attribute("name", 5, false, false));
    TableStruct ts("a", attrs, true, 2);

//    Tuple t;
//    t.data.push_back(new DataI(99));
//    t.data.push_back(new DataF(2.222));
//    t.data.push_back(new DataS("Kira!"));
//    rm.insertIntoTable(ts, t);
    vector<Condition> d;
    vector<int> range;
//    range.push_back(0);
//    range.push_back(1);
//    vector<Tuple> result;
//    rm.selectFromTableWithIndex(ts, d, range, result);
//    for (auto res:result) {
//        cout << (dynamic_cast<DataS *>(res.data[2])->x) << endl;
//    }
//    rm.selectFromTable(ts, d, result);
//    for (auto res:result) {
//        cout << (dynamic_cast<DataS *>(res.data[2])->x) << endl;
//    }
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
// create table aaa(name int unique,city char(8) unique);
// create table bbb(name int unique,city char(8) unique);
// insert into aaa values (123,'wyaa1234');
// insert into aaa values (1234,'wybb1234');
// delete from aaa where name=99999;
//select * from aaa where name2=1234;
//select * from aaa where name2=1234 and name2>10000;
//select * from aaa where name2>12 and name2<10000;

