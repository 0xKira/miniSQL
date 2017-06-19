//
// Created by Kira on 2017/6/8.
//

#ifndef MINISQL_TABLESTRUCT_H
#define MINISQL_TABLESTRUCT_H

#include <vector>
#include <string>

extern size_t blockSize;

using namespace std;

class Attribute {
public:
    string attrName;
    int type;   // -1 -> float, 0 -> int, >0 -> char
    bool unique;
    bool isIndex;

public:
    Attribute() {
    }

    Attribute(string attrName, int type, bool unique, bool isIndex) : attrName(attrName), type(type), unique(unique),
                                                                      isIndex(isIndex) {
    }
};

class TableStruct {
public:
    string tableName;
    vector<Attribute> attrs;
    bool hasIndex;
    size_t tupleNum;
    size_t tupleSize;
    size_t blockMaxRecordCount;

public:
    TableStruct() : tupleNum(0) {
    };

    TableStruct(string tableName, size_t tupleNum, vector<Attribute> attrs) : tableName(tableName), tupleNum(tupleNum),
                                                                              attrs(attrs) {
        tupleSize = 0;
        for (auto attr:attrs) {
            switch(attr.type) {
                case TYPE_FLOAT:
                case TYPE_INT:
                    tupleSize += 4;
                    break;
                default:
                    tupleSize += attr.type;
                    break;
            }
        }
        // 一个block最多有几条记录
        blockMaxRecordCount = blockSize / tupleSize;
    }
};

#endif //MINISQL_TABLESTRUCT_H
