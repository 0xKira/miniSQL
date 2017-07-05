//
// Created by Kira on 2017/6/13.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"

gd
extern const size_t blockSize;
extern BufferManager bm;

class RecordManager {
private:
    void splitTuple(const Tuple &t, char *buf);

    Tuple *resolveData(const TableStruct &table, char *data);
    bool isConditionSatisfied(const vector<Condition> &conditions, Tuple &t);
public:
    RecordManager();

    bool insertIntoTable(TableStruct &table, const Tuple &t);
    bool deleteFromTableWithIndex(TableStruct &table,
                                  const vector<Condition> &conditions,
                                  vector<int> &range,
                                  vector<Data &> &moved,
                                  vector<int> &deleted);
    bool selectFromTableWithIndex(const TableStruct &table,
                                  const vector<Condition> &conditions,
                                  vector<int> &range,
                                  vector<Tuple> &result);

    bool deleteFromTable(TableStruct &table,
                         const vector<Condition> &conditions);
    bool selectFromTable(const TableStruct &table,
                         const vector<Condition> &conditions,
                         vector<Tuple> &result);
};


#endif //MINISQL_RECORDMANAGER_H
