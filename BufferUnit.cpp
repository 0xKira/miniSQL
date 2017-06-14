//
// Created by Kira on 2017/6/10.
//

#include <cstring>
#include <iostream>
#include <fstream>
#include "BufferUnit.h"

BufferUnit::BufferUnit(const string &tableName, size_t blockSize, size_t blockNum, size_t fileSize) : blockSize(blockSize),
                                                                                               blockNum(blockNum),
                                                                                               fileSize(fileSize) {

    fileName = "./data/" + tableName + ".data";
    blockIndexInBuffer.clear();
    for (int i = 0; i < fileSize / blockSize; i++)
        blockIndexInBuffer.push_back(-1);
    blocks.clear();
}

BufferUnit::~BufferUnit() {
    fstream file(fileName, ios::in | ios::out | ios::binary);
    char *buffer = new char[fileSize];
    file.read(buffer, fileSize);
    for (auto block:blocks) {
        if (block.dirty)
            memcpy(buffer + block.indexInFile * blockSize, block.data, blockSize);
    }
    file.seekp(0, ios::beg);
    file.write(buffer, fileSize);
    file.close();
    delete[] buffer;
}

int BufferUnit::getAvailableBlock() {
    if (blocks.size() < blockNum) {
        // 还有可用的block
//        BufferDataBlock *dataBlock = new BufferDataBlock(blockSize);
//        blocks.push_back(*dataBlock);
        blocks.push_back(BufferDataBlock(blockSize));
        return blocks.size() - 1;
    }
    // 已无空余，利用clock算法找到可用的block
    while (true) {
        // 第一遍遍历
        for (int i = 0; i < blocks.size(); i++) {
            if (!blocks[i].dirty && !blocks[i].visited)
                return i;
        }
        // 第二遍遍历
        for (int i = 0; i < blocks.size(); i++) {
            // 被修改但没被访问
            if (blocks[i].dirty && !blocks[i].visited)
                return i;
            blocks[i].visited = false;
        }
    }
}

bool BufferUnit::readBlock(int blockIndexInFile, char *outBuffer) {
    if (blockIndexInFile < 0 || blockIndexInFile >= blockIndexInBuffer.size()) {
        cerr << "block index error" << endl;
        return false;
    }
    if (blockIndexInBuffer[blockIndexInFile] == -1) {
        // 该block不在内存中，将其读入内存
        int index = getAvailableBlock();
        if (index == -1) {
            cerr << "no more available block" << endl;
            return false;
        }
		char *buffer = new char(blockSize);
        // 从文件中读取内容
        fstream file(fileName, ios::in | ios::out | ios::binary);
        file.seekg(blockIndexInFile * blockSize, ios::beg);
        file.read(buffer, blockSize);
        BufferDataBlock &block = blocks[index];
        if (block.dirty) {
            // 如果已被修改则需要写回去文件
            file.seekp(block.indexInFile * blockSize, ios::beg);
            file.write(block.data, blockSize);
            block.dirty = false;
        }
        file.close();
        memcpy(block.data, buffer, blockSize);
        memcpy(outBuffer, buffer, blockSize);
        blockIndexInBuffer[blockIndexInFile] = index;
        block.indexInFile = blockIndexInFile;
        block.visited = true;
		delete[] buffer;
    } else {
        BufferDataBlock &block = blocks[blockIndexInBuffer[blockIndexInFile]];
        memcpy(outBuffer, block.data, blockSize);
        block.visited = true;
    }
    return true;
}

bool BufferUnit::writeBlock(int blockIndexInFile, char *inBuffer) {
    if (blockIndexInFile < 0 || blockIndexInFile >= blockIndexInBuffer.size()) {
        cerr << "block index error" << endl;
        return false;
    }
    if (blockIndexInBuffer[blockIndexInFile] == -1) {
        // 该block不在内存中
        int index = getAvailableBlock();
        if (index == -1) {
            cerr << "no more available block" << endl;
            return false;
        }
        // 与read时不同，不需要读取文件
        BufferDataBlock &block = blocks[index];
        if (block.dirty) {
            ofstream file(fileName, ios::binary | ios::app);
            // 如果已被修改则需要写回去文件
            file.seekp(block.indexInFile * blockSize, ios::beg);
            file.write(block.data, blockSize);
            file.close();
        }
        memcpy(block.data, inBuffer, blockSize);
        blockIndexInBuffer[blockIndexInFile] = index;
        block.indexInFile = blockIndexInFile;
        block.dirty = true;
    } else {
        BufferDataBlock &block = blocks[blockIndexInBuffer[blockIndexInFile]];
        memcpy(block.data, inBuffer, blockSize);
        block.dirty = true;
    }
    return true;
}

bool BufferUnit::deleteLastBlock() {
    int index = blockIndexInBuffer.back();
    blockIndexInBuffer.pop_back();
    vector<BufferDataBlock>::iterator itr = blocks.begin() + index;
    blocks.erase(itr);
    // 在文件中删除
    fileSize -= blockSize;
    char *buffer = new char[fileSize];
    ifstream in(fileName, ios::binary);
    in.read(buffer, fileSize);
    in.close();
    ofstream out(fileName, ios::binary);
    out.write(buffer, fileSize);
    out.close();
    delete[] buffer;
    return false;
}

bool BufferUnit::enlargeFile() {
    blockIndexInBuffer.push_back(-1);
    fstream file(fileName, ios::in | ios::out | ios::binary);
    fileSize += blockSize;
    char *buffer = new char[fileSize];
    file.read(buffer, fileSize - blockSize);
    file.seekp(0, ios::beg);
    file.write(buffer, fileSize);
    file.close();
    delete[] buffer;
}
