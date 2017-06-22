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

//select*from adf where a='adfsf'and b='adf';
//create table taname(name int,city char(8) unique);
//create index stunameidx on student (sname);
//drop table student;
//drop index stunameidx;
//insert into student values ('12345678','wy',22,'M');
//delete from student where sno ='88888888';
//execfile afddsfile;

int main(void) {
    while (true)
    {
        itp.EXEC();
    }
}