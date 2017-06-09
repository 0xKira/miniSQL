//
// Created by Kira on 2017/6/9.
//

#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H

#include <vector>
#include <string>

using namespace std;

// 管理所有表的buffer
class BufferManager {
private:
    size_t blockSize;
    size_t blockNum;

public:
    bool createTable(string tableName);
    bool deleteTable(string tableName);  // 除了删除文件还需要释放程序中相关buffer管理结构内存
    bool readBlockData(string tableName, size_t blockIndex, char *outBuffer);
    bool writeBlockData(string tableName, size_t blockIndex, char *inBuffer);
    bool deleteLastBlockOfFile(string tableName);
    bool setBlockLock(string tableName, int blockIndex, bool lock);

    BufferManager(size_t blockSize = 4096, size_t blockNum = 256) : blockSize(blockSize), blockNum(blockNum) {
    };
};

#endif //MINISQL_BUFFERMANAGER_H
