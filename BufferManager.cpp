//
// Created by Kira on 2017/6/10.
//

#include <fstream>
#include <iostream>
#include <cstring>
#include "BufferManager.h"

bool BufferManager::createTable(string tableName) {
    // 创建文件
    ofstream outfile("./data/" + tableName + ".data");
    outfile.close();
    // 创建BufferUnit
    buffers.push_back(BufferUnit(tableName, blockSize, blockNum));
    return true;
}

bool BufferManager::deleteTable(string tableName) {
    // 删除文件
    string fileName = "./data/" + tableName + ".data";
    if (remove(fileName.c_str())) {
        cerr << "can't remove file" << endl;
        return false;
    }
    // 清理相关内存
    vector<BufferUnit>::iterator index = buffers.begin() + getTableIndex(tableName);
    buffers.erase(index);
    return true;
}

bool BufferManager::readBlockData(string tableName, size_t blockIndex, char *outBuffer) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].readBlock(blockIndex, outBuffer);
}

bool BufferManager::writeBlockData(string tableName, size_t blockIndex, char *inBuffer) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].writeBlock(blockIndex, inBuffer);
}

BufferManager::BufferManager(size_t blockSize = 4096, size_t blockNum = 256) : blockSize(blockSize),
                                                                               blockNum(blockNum) {
    buffers.clear();
}

inline int BufferManager::getTableIndex(string tableName) {
    for (int i = 0; i < buffers.size(); i++) {
        if (buffers[i].tableName == tableName)
            return i;
    }
    return -1;
}
