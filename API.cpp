#include <iostream>
#include "API.h"

using namespace std;

void API::createTable(TableStruct &table) {
    cm.addTable(table);
    bm.createTable(table.tableName);
}

void API::dropTable(string tableName) {
    cm.deleteTable(tableName);
    bm.dropTable(tableName);
}

void API::createIndex(string tableName, string indexName, string attrName) {
    cm.addIndex(tableName, indexName, attrName);
}

void API::dropIndex(string indexName) {
    cm.deleteIndex(indexName);
}

vector<Tuple> &API::select(TableStruct &table, vector<Condition> &condTable) {
    vector<Tuple> *result = new vector<Tuple>;
    bool flag;
    if (table.hasIndex) {
        // to be done
        // 调用indexManager的select
    } else {
        flag = rm.selectFromTable(table, condTable, *result);
    }
    if (!flag) printf("Can't find\n");
    cout<<"sdafaf"<<endl;
    return *result;
}

void API::insertData(TableStruct &table, Tuple &tuple) {
    rm.insertIntoTable(table, tuple);
}

void API::deleteData(TableStruct &table, vector<Condition> &condTable) {
    if (table.hasIndex) {
        // to be done
        // 调用indexManager的delete
    } else {
        rm.deleteFromTable(table, condTable);
    }
    cm.writeback(table);
}