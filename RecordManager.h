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
    bool createTable(string tableName);
    bool dropTable(string tableName);
    bool insertIntoTable(const TableStruct &table);
    bool deleteFromTable(const TableStruct &table,
                         const vector<CONDITION> &conditions,
                         const vector<int> &range,
                         const vector<int> &moved,
                         const vector<int> &deleted);
    bool selectFromTable(const TableStruct &table,
                         const vector<CONDITION> &conditions,
                         const vector<int> &range,
                         vector<char *> &result);
};


#endif //MINISQL_RECORDMANAGER_H
