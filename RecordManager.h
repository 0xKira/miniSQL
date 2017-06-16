//
// Created by Kira on 2017/6/13.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"

class RecordManager {
public:
    RecordManager();
    bool insertIntoTable(const TableStruct &table);
    bool deleteFromTableWithIndex(const TableStruct &table,
                                  const vector<Condition> &conditions,
                                  const vector<int> &range,
                                  vector<Data &> &moved,
                                  vector<int> &deleted);
    bool selectFromTableWithIndex(const TableStruct &table,
                         const vector<Condition> &conditions,
                         const vector<int> &range,
                         vector<Tuple> &result);
    bool deleteFromTable(const TableStruct &table,
                         const vector<Condition> &conditions,
                         vector<Data &> &moved,
                         vector<int> &deleted);
    bool selectFromTable(const TableStruct &table,
                         const vector<Condition> &conditions,
                         vector<Tuple> &result);
};


#endif //MINISQL_RECORDMANAGER_H
