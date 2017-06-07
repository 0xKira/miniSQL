//
// Created by Kira on 2017/6/8.
//

#ifndef MINISQL_TABLESTRUCT_H
#define MINISQL_TABLESTRUCT_H

#include <vector>
#include <string>

using namespace std;

const int TYPE_INT = 0;
const int TYPE_FLOAT = -1;

class Attribute {
public:
    string attrName;
    int type;   // -1 -> float, 0 -> int, >0 -> char
    bool unique;
    bool isIndex;

public:
    Attribute() {
    }

    Attribute(string attrName, int type, bool unique, bool isIndex) :
            attrName(attrName), type(type), unique(unique), isIndex(isIndex) {
    }
};

class TableStruct {
public:
    string tableName;
    vector<Attribute> attrs;
    bool hasIndex;
    size_t tupleNum;

public:
    TableStruct() : tupleNum(0) {
    };
    TableStruct(string tableName, size_t tupleNum, vector<Attribute> attrs):
            tableName(tableName), tupleNum(tupleNum), attrs(attrs){

    }
};

#endif //MINISQL_TABLESTRUCT_H
