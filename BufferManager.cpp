//
// Created by Kira on 2017/6/10.
//

#include <fstream>
#include <iostream>
#include <cstring>
#include "BufferManager.h"

BufferManager::BufferManager(size_t blockSize = 4096, size_t blockNum = 256) : blockSize(blockSize),
                                                                               blockNum(blockNum) {
    buffers.clear();
}

inline int BufferManager::getTableIndex(string tableName) {
    string fileName = "./data/" + tableName + ".data";
    for (int i = 0; i < buffers.size(); i++) {
        if (buffers[i].fileName == fileName)
            return i;
    }
    return -1;
}

bool BufferManager::createTable(string tableName) {
    // 创建文件，如果文件已经存在就删除
    ofstream outfile("./data/" + tableName + ".data", ios::trunc);
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
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    vector<BufferUnit>::iterator itr = buffers.begin() + index;
    buffers.erase(itr);
    return true;
}

bool BufferManager::readBlockData(string tableName, size_t blockIndexInFile, char *outBuffer) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].readBlock(blockIndexInFile, outBuffer);
}

bool BufferManager::writeBlockData(string tableName, size_t blockIndexInFile, char *inBuffer) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].writeBlock(blockIndexInFile, inBuffer);
}

bool BufferManager::deleteLastBlockOfFile(string tableName) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].deleteLastBlock();
}

bool BufferManager::enlargeFile(string tableName) {
    int index = getTableIndex(tableName);
    if (index == -1) {
        return false;
    }
    return buffers[index].enlargeFile();
}

BufferManager::~BufferManager() {
}
