//
// Created by Kira on 2017/6/10.
//

#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H

#include <vector>
#include <string>
#include "BufferUnit.h"

using namespace std;

// 管理所有表的buffer
class BufferManager {
private:
    vector<BufferUnit> buffers;

    size_t blockSize;
    size_t blockNum;

private:
    inline int getTableIndex(string tableName);
    bool buildBufferUnits();

public:
    bool createTable(const string &tableName);
    bool dropTable(const string &tableName);  // 除了删除文件还需要释放程序中相关buffer管理结构内存
    // 下面的blockIndex是指文件中的index
    bool readBlockData(const string &tableName, size_t blockIndexInFile, char *outBuffer);
    bool writeBlockData(const string &tableName, size_t blockIndexInFile, char const *inBuffer);
    bool deleteLastBlockOfFile(const string &tableName);
    bool enlargeFile(const string &tableName);

    BufferManager(size_t blockSize, size_t blockNum);
    ~BufferManager();
};

#endif //MINISQL_BUFFERMANAGER_H
