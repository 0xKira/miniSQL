//
// Created by Kira on 2017/6/13.
//

#include "RecordManager.h"

RecordManager::RecordManager() {

}

bool RecordManager::createTable(const string &tableName) {
    return false;
}

bool RecordManager::dropTable(const string &tableName) {
    return false;
}

bool RecordManager::insertIntoTable(const TableStruct &table) {
    return false;
}

bool
RecordManager::deleteFromTableWithIndex(const TableStruct &table, const vector<Condition> &conditions,
                                        const vector<int> &range, vector<Data &> &moved, vector<int> &deleted) {

    return false;
}

bool
RecordManager::selectFromTableWithIndex(const TableStruct &table, const vector<Condition> &conditions,
                                        const vector<int> &range, vector<Tuple> &result) {

    return false;
}

bool
RecordManager::deleteFromTable(const TableStruct &table, const vector<Condition> &conditions, vector<Data &> &moved,
                               vector<int> &deleted) {

    return false;
}

bool
RecordManager::selectFromTable(const TableStruct &table, const vector<Condition> &conditions, vector<Tuple> &result) {
    return false;
}

