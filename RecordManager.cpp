//
// Created by Kira on 2017/6/13.
//

#include <algorithm>
#include "RecordManager.h"

RecordManager::RecordManager() {
}

bool RecordManager::insertIntoTable(TableStruct &table, const Tuple &t) {
    // 一个block至多存几条记录
    size_t blockToInsert;
    char *buf = new char[blockSize];
    char *ptr;
    if (table.tupleNum % table.blockMaxRecordCount == 0) {
        bm.enlargeFile(table.tableName);
    }
    blockToInsert = table.tupleNum / table.blockMaxRecordCount;
    if(!bm.readBlockData(table.tableName, blockToInsert, buf))
        return false;
    // 解析tuple至buf
    ptr = table.tupleNum % table.blockMaxRecordCount * table.tupleSize + buf;
    splitTuple(t, ptr);
    bool ret = bm.writeBlockData(table.tableName, blockToInsert, buf);
    table.tupleNum++;
    delete[] buf;
    if (table.hasIndex) {
        // 调用indexManager更新index
    }
    return ret;
}

bool
RecordManager::deleteFromTableWithIndex(TableStruct &table, const vector<Condition> &conditions,
                                        vector<int> &range, vector<Data &> &moved, vector<int> &deleted) {
    if (table.tupleNum == 0)
        return false;
    // 第几个block，block中的第几条记录
    size_t indexInFile, indexInBlock;
    char *buf = new char[blockSize];
    char *lastBlock = new char[blockSize];
    sort(range.begin(), range.end());

    // 获得最后一个block的数据
    bm.readBlockData(table.tableName, (table.tupleNum - 1) / table.blockMaxRecordCount, lastBlock);
    for (int i = range.size() - 1; i >= 0;) {
        indexInFile = range[i] / table.blockMaxRecordCount;
        bm.readBlockData(table.tableName, indexInFile, buf);
        do {
            // 读取一条记录
            indexInBlock = range[i] % table.blockMaxRecordCount;
            Tuple *t = resolveData(table, buf + indexInBlock * table.tupleSize);
            // 判断这条记录是否需要删除
            if (isConditionSatisfied(conditions, *t)) {
                if (range[i] != table.tupleNum - 1) {
                    // 如果不是最后一条，那么将最后一条移动到被删除的地方，如果是最后一条就很爽了，基本啥也不干
                    memcpy(buf + indexInBlock * table.tupleSize,
                           lastBlock + (table.tupleNum - 1) % table.blockMaxRecordCount * table.tupleSize,
                           table.tupleSize);
                    bm.writeBlockData(table.tableName, indexInFile, buf);
                    // 记录被移动的以及删除的位置，供indexManager更新
                    // to be done
                }
                table.tupleNum--;
                if (table.tupleNum % table.blockMaxRecordCount == 0) {
                    // 此时表已空余一个block
                    // 更新lastBlock
                    bm.readBlockData(table.tableName, (table.tupleNum - 1) / table.blockMaxRecordCount, lastBlock);
                    // 删除最后的block
                    bm.deleteLastBlockOfFile(table.tableName);
                }
            }
            delete t;
            i--;
        } while ((range[i] / table.blockMaxRecordCount) == indexInFile);
    }
    delete[] buf;
    delete[] lastBlock;
    return true;
}

bool
RecordManager::deleteFromTable(TableStruct &table, const vector<Condition> &conditions) {
    if (table.tupleNum == 0)
        return false;
    // 第几个block，block中的第几条记录
    size_t blockNum, recordCount;
    char *buf = new char[blockSize];
    char *lastBlock = new char[blockSize];

    // 获得最后一个block的数据
    blockNum = (table.tupleNum - 1) / table.blockMaxRecordCount + 1;
    bm.readBlockData(table.tableName, blockNum - 1, lastBlock);
    // 从后往前读取block
    for (int i = blockNum - 1; i >= 0; i--) {
        bm.readBlockData(table.tableName, i, buf);
        recordCount = (i == blockNum - 1) ? table.tupleNum % table.blockMaxRecordCount : table.blockMaxRecordCount;
        // 从后往前遍历所有的记录
        for (int j = recordCount - 1; j >= 0; j--) {
            Tuple *t = resolveData(table, buf + j * table.tupleSize);
            // 判断这条记录是否需要删除
            if (isConditionSatisfied(conditions, *t)) {
                if (j != table.tupleNum - 1) {
                    // 如果不是最后一条，那么将最后一条移动到被删除的地方，如果是最后一条就很爽了，基本啥也不干
                    memcpy(buf + j * table.tupleSize,
                           lastBlock + (table.tupleNum - 1) % table.blockMaxRecordCount * table.tupleSize,
                           table.tupleSize);
                    bm.writeBlockData(table.tableName, i, buf);
                }
                table.tupleNum--;
                if (table.tupleNum % table.blockMaxRecordCount == 0) {
                    // 此时表已空余一个block
                    // 更新lastBlock
                    bm.readBlockData(table.tableName, ((int) table.tupleNum - 1) / table.blockMaxRecordCount,
                                     lastBlock);
                    // 删除最后的block
                    bm.deleteLastBlockOfFile(table.tableName);
                }
            }
            delete t;
        }
    }
    delete[] buf;
    delete[] lastBlock;

    return true;
}

bool
RecordManager::selectFromTableWithIndex(const TableStruct &table, const vector<Condition> &conditions,
                                        vector<int> &range, vector<Tuple> &result) {
    if (table.tupleNum == 0)
        return false;
    // 第几个block，block中的第几条记录
    size_t indexInFile, indexInBlock;
    char *buf = new char[blockSize];
    sort(range.begin(), range.end());

    for (int i = 0; i < range.size();) {
        indexInFile = range[i] / table.blockMaxRecordCount;
        bm.readBlockData(table.tableName, indexInFile, buf);
        do {
            indexInBlock = range[i] % table.blockMaxRecordCount;
            Tuple *t = resolveData(table, buf + indexInBlock * table.tupleSize);
            result.push_back(*t);
            delete t;
            i++;
        } while (i < range.size() && ((range[i] / table.blockMaxRecordCount) == indexInFile));
    }
    delete[] buf;
    vector<Tuple>::iterator itr = result.begin();
    for (int i = 0; i < result.size(); i++) {
        if(!isConditionSatisfied(conditions, result[i]))
            result.erase(itr + i);
    }
    return true;
}

bool
RecordManager::selectFromTable(const TableStruct &table, const vector<Condition> &conditions, vector<Tuple> &result) {
    if (table.tupleNum == 0)
        return false;
    // 第几个block，block中的第几条记录
    size_t blockNum, selectCount = 0;
    char *buf = new char[blockSize];

    blockNum = (table.tupleNum - 1) / table.blockMaxRecordCount + 1;
    for (size_t i = 0; i < blockNum; i++) {
        bm.readBlockData(table.tableName, i, buf);
        // 遍历所有的记录
        for (int j = 0; j < table.blockMaxRecordCount; j++) {
            Tuple *t = resolveData(table, buf + j * table.tupleSize);
            if (isConditionSatisfied(conditions, *t))
                result.push_back(*t);
            delete t;
            selectCount++;
            if (selectCount == table.tupleNum)
                break;
        }
        i++;
    }
    delete[] buf;
    return true;
}

void RecordManager::splitTuple(const Tuple &t, char *buf) {
    for (auto data:t.data) {
        switch (data->type) {
            case TYPE_INT:
                *(int *) buf = dynamic_cast<DataI *>(data)->x;
                buf += 4;
                break;
            case TYPE_FLOAT:
                *(float *) buf = dynamic_cast<DataF *>(data)->x;
                buf += 4;
                break;
            default:
                string &tmp = dynamic_cast<DataS *>(data)->x;
                memcpy(buf, tmp.c_str(), tmp.length());
                buf += tmp.length();
                break;
        }
    }
}

Tuple *RecordManager::resolveData(const TableStruct &table, char *data) {
    // 根据TableStruct将数据解析至tuple
    Tuple *t = new Tuple;
    Data *d;
    t->data.clear();
    for (auto attr:table.attrs) {
        switch (attr.type) {
            case TYPE_FLOAT:
                d = new DataF(*(float *) data);
                data += 4;
                break;
            case TYPE_INT:
                d = new DataI(*(int *) data);
                data += 4;
                break;
            default:
                string s(data, 0, (unsigned int) attr.type);
                d = new DataS(s);
                data += attr.type;
                break;
        }
        t->data.push_back(d);
    }
    return t;
}

// this function is too ugly
bool RecordManager::isConditionSatisfied(const vector<Condition> &conditions, Tuple &t) {
    for (int i = 0; i < conditions.size(); i++) {
//    for (auto con:conditions) {
        // EQ, LEQ, L, GEQ, G, NEQ
        const Condition &con = conditions[i];
        switch (con.flag) {
            case EQ:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x != dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x != dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x != dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
            case LEQ:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x > dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x > dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x > dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
            case L:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x >= dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x >= dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x >= dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
            case GEQ:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x < dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x < dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x < dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
            case G:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x <= dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x <= dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x <= dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
            case NEQ:
                switch (con.d->type) {
                    case TYPE_INT:
                        if (dynamic_cast<DataI *>(t.data[con.attrIndex])->x == dynamic_cast<DataI *>(con.d)->x)
                            return false;
                        break;
                    case TYPE_FLOAT:
                        if (dynamic_cast<DataF *>(t.data[con.attrIndex])->x == dynamic_cast<DataF *>(con.d)->x)
                            return false;
                        break;
                    default:
                        if (dynamic_cast<DataS *>(t.data[con.attrIndex])->x == dynamic_cast<DataS *>(con.d)->x)
                            return false;
                        break;
                }
                break;
        }
    }
    return true;
}

