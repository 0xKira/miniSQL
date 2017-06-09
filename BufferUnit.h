//
// Created by Kira on 2017/6/9.
//

#ifndef MINISQL_BUFFERUNIT_H
#define MINISQL_BUFFERUNIT_H

#include <vector>
#include <string>

using namespace std;

class BufferDataBlock {
public:
    char *data;
    bool visited;
    bool valid;
    bool dirty;
    bool lock;
    int indexInFile; // 文件中第几个block
    size_t size;     // data的大小

    BufferDataBlock(size_t size) : visited(false), valid(false), dirty(false), lock(false), indexInFile(-1) {
        data = new char[size];
        this->size = size;
    }

    BufferDataBlock(const BufferDataBlock &block) : visited(false), valid(false), dirty(false), lock(false),
                                                    indexInFile(-1) {
        data = new char[block.size];
        size = block.size;
    };

    ~BufferDataBlock() {
        delete[] data;
    }
};

// 管理一张表的所有buffer
class BufferUnit {
private:
    size_t blockSize;
    size_t blockNum;
    string tableName;
    size_t fileSize;

public:
    bool readBlock(int index, char *outBuffer);
    bool writeBlock(int index, char *inBuffer);
    bool deleteLastBlock();
    bool setBlockLock(int index, bool lock);
    // 这里fileSize是0有问题吗？
    BufferUnit(string tableName, size_t blockSize, size_t blockNum) : tableName(tableName), blockSize(blockSize),
                                                                      blockNum(blockNum), fileSize(0) {
    }

    ~BufferUnit() {
    };
};

#endif //MINISQL_BUFFERUNIT_H
