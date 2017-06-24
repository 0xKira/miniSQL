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
    range.push_back(0);
    range.push_back(1);
    vector<Tuple> result;
    rm.selectFromTableWithIndex(ts, d, range, result);
    for (auto res:result) {
        cout << (dynamic_cast<DataS *>(res.data[2])->x) << endl;
    }
    rm.selectFromTable(ts, d, result);
    for (auto res:result) {
        cout << (dynamic_cast<DataS *>(res.data[2])->x) << endl;
    }
    while (true) {
        itp.EXEC();
    }

}
//create table taname(name2 int unique,city2 char(8) unique);
//insert into taname values (123,'wyaa1234');
//insert into taname values (1234,'wybb1234');
