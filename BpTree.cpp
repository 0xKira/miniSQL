#include "BpTree.h"
#include "RecordManager.h"
#include <iostream>
#include <fstream>
using namespace std;
#define NODESIZE 1024
#define HEADLEN 13

BpTree::BpTree(const TableStruct& table, const string& index, const Attribute& attr) :
	tableName(table.tableName),
	indexName(index)
{
	string filename("./data/" + index + ".index");
	fstream indexFile(filename, ios::in);
	if (!indexFile)
	{
		size = 0;
		type = attr.type;
		buf = "";
		indexFile.open(filename, ios::out);
		indexFile << table.tableName << endl;
		indexFile << attr.type << " " << 0 << endl;
	}
	else
		throw TableException("Index exists!");
	indexFile.close();
}

BpTree::BpTree(const string & index) : indexName(index)
{
	string filename("./data/" + index + ".index");
	fstream indexFile(filename, ios::in);
	if (!indexFile)
	{
		throw TableException("Index not exists!");
	}
	else
	{
		string tableName, temp;
		short type;
		indexFile >> tableName;
		indexFile >> type >> root;
		size_t moduleSize = 4;
		if (type < 1) moduleSize += 4; else moduleSize += type;
		indexFile >> temp;
		size = temp.length / NODESIZE;
		buf = (char*)malloc(sizeof(char)*temp.length);
		strcpy(buf, temp.c_str);
	}
	indexFile.close();
}

BpTree::~BpTree()
{
	string filename("./data" + indexName + ".index");
	fstream indexFile(filename, ios::out);
	indexFile << tableName << endl;
	indexFile << type << " " << root << endl;
	indexFile << buf << endl;
	indexFile.close;
}

void BpTree::initialize(Data* data, int pos)
{
	buf = (char*)malloc(sizeof(char)*NODESIZE);
	size++;
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	buf[0] = 1;				//leaf node
	*(int*)(buf + 1) = 0;	//node pos
	*(int*)(buf + 5) = -1;	//father pos
	*(int*)(buf + 9) = 1;	//key number
	*(int*)(buf + 13) = pos;	//pos in table or child pos
	if (type == -1)
		*(int*)(buf + HEADLEN) = ((DataF*)data)->x;
	else if (type == 0)
		*(int*)(buf + HEADLEN) = ((DataI*)data)->x;
	else
		memcpy(buf + HEADLEN, &(((DataS*)data)->x), type*sizeof(char));
	*(int*)(buf + HEADLEN + moduleSize) = -1;	//next node pos
}

int BpTree::find(Data * data)
{
	char* currentNode = new char[NODESIZE];
	memcpy(currentNode, buf + root*NODESIZE, NODESIZE);
	bool ifLeaf = (bool)currentNode[0];
	int keyNum = *(int*)(currentNode + 9);
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	while (!ifLeaf)
	{
		for (i = 0; i < keyNum; i++)
		{
			int valuePos = HEADLEN + i * moduleSize + 4;
			int keyPos = valuePos - 4;
			if (type == -1 && ((DataF*)data)->x < *(float*)(currentNode + valuePos))
				break;
			if (type == 0 && ((DataI*)data)->x < *(int*)(currentNode + valuePos))
				break;
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, currentNode + valuePos, type);
				if (((DataS*)data)->x.compare(tempString) < 0)
					break;
			}
		}
		if (i == keyNum)
			keyPos += moduleSize;
		int position = *(int*)(currentNode + keyPos);
		if (position = -1) return -1;
		memcpy(currentNode, buf + position * NODESIZE, NODESIZE);
		ifLeaf = (bool)currentNode[0];
		keyNum = *(int*)(currentNode + 9);
	}
	for (i = 0; i < keyNum; i++)
	{
		int valuePos = HEADLEN + i * moduleSize + 4;
		int keyPos = valuePos - 4;
		if (type == -1 && ((DataF*)data)->x == *(float*)(currentNode + valuePos))
		{
			delete[] currentNode;
			return *(int*)(currentNode + keyPos);
		}
		if (type == 0 && ((DataI*)data)->x == *(int*)(currentNode + valuePos))
		{
			delete[] currentNode;
			return *(int*)(currentNode + keyPos);
		}
		if (type > 0)
		{
			char* tempString = new char[type];
			memcpy(tempString, currentNode + valuePos, type);
			if (((DataS*)data)->x.compare(tempString) == 0)
			{
				delete[] currentNode;
				return *(int*)(currentNode + keyPos);
			}
		}
	}
	delete[] currentNode;
	return -1;
}

vector<int> BpTree::range(Data * data, Data* up)
{
	vector<int> arr;
	char* currentNode = new char[NODESIZE];
	memcpy(currentNode, buf + root*NODESIZE, NODESIZE);
	bool ifLeaf = (bool)currentNode[0];
	int keyNum = *(int*)(currentNode + 9);
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	while (!ifLeaf)
	{
		for (i = 0; i < keyNum; i++)
		{
			int valuePos = HEADLEN + i * moduleSize + 4;
			int keyPos = valuePos - 4;
			if (type == -1 && ((DataF*)data)->x < *(float*)(currentNode + valuePos))
				break;
			if (type == 0 && ((DataI*)data)->x < *(int*)(currentNode + valuePos))
				break;
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, currentNode + valuePos, type);
				if (((DataS*)data)->x.compare(tempString) < 0)
					break;
			}
		}
		if (i == keyNum)
			keyPos += moduleSize;
		int position = *(int*)(currentNode + keyPos);
		if (position = -1) return arr;
		memcpy(currentNode, buf + position * NODESIZE, NODESIZE);
		ifLeaf = (bool)currentNode[0];
		keyNum = *(int*)(currentNode + 9);
	}
	while (true)
	{
		for (i = 0; i < keyNum; i++)
		{
			int valuePos = HEADLEN + i * moduleSize + 4;
			int keyPos = valuePos - 4;
			if (type == -1 && ((DataF*)data)->x <= *(float*)(currentNode + valuePos))
					arr.push_back(*(int*)(currentNode + keyPos));
			if (type == -1 && ((DataF*)up)->x > *(float*)(currentNode + valuePos))
			{
				delete[] currentNode;
				return arr;
			}
			if (type == 0 && ((DataI*)data)->x <= *(int*)(currentNode + valuePos))
				arr.push_back(*(int*)(currentNode + keyPos));
			if (type == 0 && ((DataI*)up)->x > *(int*)(currentNode + valuePos))
			{
				delete[] currentNode;
				return arr;
			}
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, currentNode + valuePos, type);
				if (((DataS*)data)->x.compare(tempString) <= 0)
				{
					arr.push_back(*(int*)(currentNode + keyPos));
				}
				if (((DataS*)up)->x.compare(tempString) > 0)
				{
					delete[] currentNode;
					return arr;
				}
			}
		}
		int position = *(int*)(currentNode + HEADLEN + *(int*)(currentNode+9)*moduleSize);
		if (position = -1) return arr;
		memcpy(currentNode, buf + position * NODESIZE, NODESIZE);
	}
	delete[] currentNode;
	return arr;
}
//buf Ã»ÓÐ¼°Ê±¸üÐÂ°¡°¡°¡°¡°¡°¡°¡°¡°¡°¡°¡
void BpTree::insert(Data * data, int pos)
{
	if (size == 0)
		initialize(data, pos);
	else
	{
		char* currentNode = new char[NODESIZE];
		memcpy(currentNode, buf + root*NODESIZE, NODESIZE);
		bool ifLeaf = (bool)currentNode[0];
		int keyNum = *(int*)(currentNode + 9);
		size_t moduleSize = 4;
		int keyPos, i;
		if (type < 1) moduleSize += 4; else moduleSize += type;
		while (!ifLeaf)
		{
			for (i = 0; i < keyNum; i++)
			{
				int valuePos = HEADLEN + i * moduleSize + 4;
				int keyPos = valuePos - 4;
				if (type == -1 && ((DataF*)data)->x < *(float*)(currentNode + valuePos))
					break;
				if (type == 0 && ((DataI*)data)->x < *(int*)(currentNode + valuePos))
					break;
				if (type > 0)
				{
					char* tempString = new char[type];
					memcpy(tempString, currentNode + valuePos, type);
					if (((DataS*)data)->x.compare(tempString) < 0)
						break;
				}
			}
			if (i == keyNum)
				keyPos += moduleSize;
			int position = *(int*)(currentNode + keyPos);
			if (position == -1)
			{
				char* newNode = new char[NODESIZE];
				newNode[0] = 1;
				*(int*)(newNode + 1) = size++;
				*(int*)(newNode + 5) = *(int*)(currentNode + 1);
				*(int*)(newNode + 9) = 1;
				*(int*)(newNode + 13) = 0;
				if (type = -1) *(float*)(newNode + HEADLEN) = ((DataF*)data)->x;
				if (type = 0) *(int*)(newNode + HEADLEN) = ((DataI*)data)->x;
				if (type > 0) memcpy(newNode + HEADLEN, &(((DataS*)data)->x), type * sizeof(char));
				*(int*)(newNode + HEADLEN + moduleSize) = -1;
				*(int*)(currentNode + keyPos) = size - 1;
				saveToBuf(newNode);
				delete[] newNode;
				return;
			}
			memcpy(currentNode, buf + position * NODESIZE, NODESIZE);
			ifLeaf = (bool)currentNode[0];
			keyNum = *(int*)(currentNode + 9);
		}
		int capacity = (NODESIZE - 4 - HEADLEN) / moduleSize;
		if (keyNum < capacity)
		{
			(*(int*)(currentNode + 9))++;
			if (type == -1)
			{
				int i;
				for (i = 0; ((DataF*)data)->x < *(float*)(currentNode + HEADLEN + i*moduleSize + 4); i++);
				for (int j = keyNum; j > i; j--)
					memcpy(currentNode + HEADLEN + j*moduleSize, currentNode + HEADLEN + (j - 1)*moduleSize, moduleSize);
				*(int*)(currentNode + HEADLEN + i*moduleSize) = pos;
				*(float*)(currentNode + HEADLEN + i*moduleSize + 4) = ((DataF*)data)->x;
			}
			if (type == 0)
			{
				int i;
				for (i = 0; ((DataI*)data)->x < *(int*)(currentNode + HEADLEN + i*moduleSize + 4); i++);
				for (int j = keyNum; j > i; j--)
					memcpy(currentNode + HEADLEN + j*moduleSize, currentNode + HEADLEN + (j - 1)*moduleSize, moduleSize);
				*(int*)(currentNode + HEADLEN + i*moduleSize) = pos;
				*(int*)(currentNode + HEADLEN + i*moduleSize + 4) = ((DataI*)data)->x;
			}
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, currentNode + HEADLEN + 4, type);
				int i;
				for (i = 0; ((DataS*)data)->x.compare(tempString) < 0; i++)
					memcpy(tempString, currentNode + HEADLEN + i*moduleSize, type);
				for (int j = keyNum; j > i; j--)
					memcpy(currentNode + HEADLEN + j*moduleSize, currentNode + HEADLEN + (j - 1)*moduleSize, moduleSize);
				*(int*)(currentNode + HEADLEN + i*moduleSize) = pos;
				memcpy(currentNode + HEADLEN + i*moduleSize + 4, &((DataS*)data)->x, type * sizeof(char));
			}
			saveToBuf(currentNode);
			delete[] currentNode;
		}
		else
		{
			char* newNode = new char[NODESIZE];
			char* tempNode = new char[NODESIZE * 2];
			for (int i = 0; i < capacity; i++)
				memcpy(tempNode + i*moduleSize, currentNode + HEADLEN + i*moduleSize, moduleSize);
			if (type == -1)
			{
				int i;
				for (i = 0; ((DataF*)data)->x < *(float*)(tempNode + i*moduleSize + 4); i++);
				for (int j = keyNum; j > i; j--)
					memcpy(tempNode + j*moduleSize, tempNode + (j - 1)*moduleSize, moduleSize);
				*(int*)(tempNode + i*moduleSize) = pos;
				*(float*)(tempNode + i*moduleSize + 4) = ((DataF*)data)->x;
			}
			if (type == 0)
			{
				int i;
				for (i = 0; ((DataI*)data)->x < *(int*)(tempNode + i*moduleSize + 4); i++);
				for (int j = keyNum; j > i; j--)
					memcpy(tempNode + j*moduleSize, tempNode + (j - 1)*moduleSize, moduleSize);
				*(int*)(tempNode + i*moduleSize) = pos;
				*(int*)(tempNode + i*moduleSize + 4) = ((DataI*)data)->x;
			}
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, tempNode + 4, type);
				int i;
				for (i = 0; ((DataS*)data)->x.compare(tempString) < 0; i++)
					memcpy(tempString, tempNode + (i + 1)*moduleSize + 4, type);
				delete[] tempString;
				for (int j = keyNum; j > i; j--)
					memcpy(tempNode + j*moduleSize, tempNode + (j - 1)*moduleSize, moduleSize);
				*(int*)(tempNode + i*moduleSize) = pos;
				memcpy(tempNode + i*moduleSize + 4, &((DataS*)data)->x, type * sizeof(char));
			}
			newNode[0] = 1;
			*(int*)(newNode + 1) = size++;
			*(int*)(newNode + 5) = *(int*)(currentNode + 5);
			*(int*)(newNode + 9) = *(int*)(currentNode + 9) = capacity / 2;
			memcpy(newNode + HEADLEN + capacity*moduleSize, currentNode + HEADLEN + capacity*moduleSize, 4);
			memcpy(currentNode + HEADLEN + capacity*moduleSize, newNode + 1, 4);
			memset(currentNode + HEADLEN, 0, NODESIZE - HEADLEN);
			memcpy(currentNode + HEADLEN, tempNode, (capacity / 2)*moduleSize);
			memcpy(newNode + HEADLEN, tempNode + (capacity / 2)*moduleSize, (capacity - capacity / 2)*moduleSize);
			if (type == -1)
				insertInParent(currentNode, *(float*)(tempNode + (capacity / 2)*moduleSize + 4), newNode);
			if (type == 0)
				insertInParent(currentNode, *(int*)(tempNode + (capacity / 2)*moduleSize + 4), newNode);
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, (tempNode + (capacity / 2)*moduleSize + 4), type);
				insertInParent(currentNode, tempString, newNode, type);
			}
			saveToBuf(newNode);
			delete[] newNode; delete[] tempNode;
		}
	}
}

void BpTree::insertInParent(char * node, float data, char* newNode)
{
	size_t moduleSize = 4;
	int keyPos, i;
	int capacity = (NODESIZE - 4 - HEADLEN) / moduleSize;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	if (*(int*)(node + 5) == -1)
	{
		char* tempNode = new char[NODESIZE];
		tempNode[0] = 0;//non-leaf
		*(int*)(tempNode + 1) = size++;
		*(int*)(tempNode + 5) = -1;
		root = size - 1;
		*(int*)(tempNode + 9) = 1;
		*(int*)(tempNode + HEADLEN) = *(int*)(node + 1);
		*(float*)(tempNode + HEADLEN + 4) = data;
		*(int*)(tempNode + HEADLEN + moduleSize) = *(int*)(newNode + 1);
		saveToBuf(tempNode);
		delete[] tempNode;
		return;
	}
	int p = *(int*)(node + 5);
	char* parent = new char[NODESIZE];
	memcpy(parent, buf + p*NODESIZE, NODESIZE);
	int& keyNum = *(int*)(parent + 9);
	if (keyNum < capacity)
	{
		*(float*)(parent + HEADLEN + (keyNum++) *moduleSize + 4) = data;
		*(int*)(parent + HEADLEN + keyNum*moduleSize) = *(int*)(newNode + 1);
		saveToBuf(parent);
		delete[] parent;
	}
	else
	{
		char* tempNode = new char[NODESIZE * 2];
		memcpy(tempNode, parent + HEADLEN, capacity*moduleSize);
		*(int*)(tempNode + capacity*moduleSize + 4) = data;
		*(int*)(tempNode + (capacity + 1)*moduleSize) = *(int*)(newNode + 1);
		memset(parent + HEADLEN, 0, NODESIZE - HEADLEN);
		memcpy(parent + HEADLEN, tempNode, ((capacity / 2) - 1)*moduleSize + 4);
		*(int*)(parent + 9) = capacity / 2 - 1;
		char* nnNode = new char[NODESIZE];
		nnNode[0] = 0;
		*(int*)(nnNode + 1) = size++;
		*(int*)(nnNode + 5) = *(int*)(parent + 5);
		*(int*)(nnNode + 9) = capacity - capacity / 2;
		memcpy(nnNode + HEADLEN, tempNode + (capacity / 2)*moduleSize, ((capacity - capacity / 2)*moduleSize + 4));
		float temp = *(float*)(tempNode + (capacity / 2)*moduleSize + 4);
		saveToBuf(parent); saveToBuf(nnNode);
		insertInParent(parent, temp, nnNode);
		delete[] parent; delete[] nnNode;
	}
	return;
}

void BpTree::insertInParent(char* node, char* data, char* newNode, int length)
{
	size_t moduleSize = 4;
	int keyPos, i;
	int capacity = (NODESIZE - 4 - HEADLEN) / moduleSize;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	if (*(int*)(node + 1) == root)
	{
		char* tempNode = new char[NODESIZE];
		tempNode[0] = 0;//non-leaf
		*(int*)(tempNode + 1) = size++;
		*(int*)(tempNode + 5) = -1;
		root = size - 1;
		*(int*)(tempNode + 9) = 1;
		*(int*)(tempNode + HEADLEN) = *(int*)(node + 1);
		memcpy(tempNode + HEADLEN + 4, data, length);
		*(int*)(tempNode + HEADLEN + moduleSize) = *(int*)(newNode + 1);
		saveToBuf(tempNode);
		delete[] tempNode;
		return;
	}
	int p = *(int*)(node + 5);
	char* parent = new char[NODESIZE];
	memcpy(parent, buf + p*NODESIZE, NODESIZE);
	int& keyNum = *(int*)(parent + 9);
	if (keyNum < capacity)
	{
		memcpy(parent + HEADLEN + (keyNum++) *moduleSize + 4, data, length);
		*(int*)(parent + HEADLEN + keyNum*moduleSize) = *(int*)(newNode + 1);
		saveToBuf(parent);
		delete[] parent;
	}
	else
	{
		char* tempNode = new char[NODESIZE * 2];
		memcpy(tempNode, parent + HEADLEN, capacity*moduleSize);
		memcpy(tempNode + capacity*moduleSize + 4, data, length);
		*(int*)(tempNode + (capacity + 1)*moduleSize) = *(int*)(newNode + 1);
		memset(parent + HEADLEN, 0, NODESIZE - HEADLEN);
		memcpy(parent + HEADLEN, tempNode, ((capacity / 2) - 1)*moduleSize + 4);
		*(int*)(parent + 9) = capacity / 2 - 1;
		char* nnNode = new char[NODESIZE];
		nnNode[0] = 0;
		*(int*)(nnNode + 1) = size++;
		*(int*)(nnNode + 5) = *(int*)(parent + 5);
		*(int*)(nnNode + 9) = capacity - capacity / 2;
		memcpy(nnNode + HEADLEN, tempNode + (capacity / 2)*moduleSize, ((capacity - capacity / 2)*moduleSize + 4));
		char* temp = new char[length];
		memcpy(temp, tempNode + (capacity / 2)*moduleSize + 4, length);
		saveToBuf(parent); saveToBuf(nnNode);
		insertInParent(parent, temp, nnNode, length);
		delete[] parent; delete[] nnNode; delete[] temp;
	}
	return;
}

void BpTree::insertInParent(char * node, int data, char* newNode)
{
	size_t moduleSize = 4;
	int keyPos, i;
	int capacity = (NODESIZE - 4 - HEADLEN) / moduleSize;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	if (*(int*)(node + 5) == -1)
	{
		char* tempNode = new char[NODESIZE];
		tempNode[0] = 0;//non-leaf
		*(int*)(tempNode + 1) = size++;
		*(int*)(tempNode + 5) = -1;
		root = size - 1;
		*(int*)(tempNode + 9) = 1;
		*(int*)(tempNode + HEADLEN) = *(int*)(node + 1);
		*(int*)(tempNode + HEADLEN + 4) = data;
		*(int*)(tempNode + HEADLEN + moduleSize) = *(int*)(newNode + 1);
		saveToBuf(tempNode);
		delete[] tempNode;
		return;
	}
	int p = *(int*)(node + 5);
	char* parent = new char[NODESIZE];
	memcpy(parent, buf + p*NODESIZE, NODESIZE);
	int& keyNum = *(int*)(parent + 9);
	if (keyNum < capacity)
	{
		*(int*)(parent + HEADLEN + (keyNum++) *moduleSize + 4) = data;
		*(int*)(parent + HEADLEN + keyNum*moduleSize) = *(int*)(newNode + 1);
		saveToBuf(parent);
		delete[] parent;
	}
	else
	{
		char* tempNode = new char[NODESIZE * 2];
		memcpy(tempNode, parent + HEADLEN, capacity*moduleSize);
		*(int*)(tempNode + capacity*moduleSize + 4) = data;
		*(int*)(tempNode + (capacity + 1)*moduleSize) = *(int*)(newNode + 1);
		memset(parent + HEADLEN, 0, NODESIZE - HEADLEN);
		memcpy(parent + HEADLEN, tempNode, ((capacity / 2) - 1)*moduleSize + 4);
		*(int*)(parent + 9) = capacity / 2 - 1;
		char* nnNode = new char[NODESIZE];
		nnNode[0] = 0;
		*(int*)(nnNode + 1) = size++;
		*(int*)(nnNode + 5) = *(int*)(parent + 5);
		*(int*)(nnNode + 9) = capacity - capacity / 2;
		memcpy(nnNode + HEADLEN, tempNode + (capacity / 2)*moduleSize, ((capacity - capacity / 2)*moduleSize + 4));
		int temp = *(float*)(tempNode + (capacity / 2)*moduleSize + 4);
		saveToBuf(parent); saveToBuf(nnNode);
		insertInParent(parent, temp, nnNode);
		delete[] parent; delete[] nnNode;
	}
	return;
}

void BpTree::saveToBuf(char* node)
{
	memcpy(buf + *(int*)(node + 1)*NODESIZE, node, NODESIZE);
	return;
}

void BpTree::deleteData(Data* data)
{
	char* currentNode = new char[NODESIZE];
	memcpy(currentNode, buf + root*NODESIZE, NODESIZE);
	bool ifLeaf = (bool)currentNode[0];
	int keyNum = *(int*)(currentNode + 9);
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	while (!ifLeaf)
	{
		for (i = 0; i < keyNum; i++)
		{
			int valuePos = HEADLEN + i * moduleSize + 4;
			int keyPos = valuePos - 4;
			if (type == -1 && ((DataF*)data)->x < *(float*)(currentNode + valuePos))
				break;
			if (type == 0 && ((DataI*)data)->x < *(int*)(currentNode + valuePos))
				break;
			if (type > 0)
			{
				char* tempString = new char[type];
				memcpy(tempString, currentNode + valuePos, type);
				if (((DataS*)data)->x.compare(tempString) < 0)
					break;
			}
		}
		if (i == keyNum)
			keyPos += moduleSize;
		int position = *(int*)(currentNode + keyPos);
		if (position = -1) throw "fail to find";
		memcpy(currentNode, buf + position * NODESIZE, NODESIZE);
		ifLeaf = (bool)currentNode[0];
		keyNum = *(int*)(currentNode + 9);
	}
	deleteEntry(currentNode, data);
}

void BpTree::deleteEntry(char * node, Data * data)
{
	int i;
	int& keyNum = *(int*)(node + 9);
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	char* tempString = new char[256];
	int capacity = (NODESIZE - 4 - HEADLEN) / moduleSize;
	for (i = 0; i < keyNum; i++)
	{
		if (type == -1 && *(float*)(node + HEADLEN + i*moduleSize + 4) == ((DataF*)data)->x ||
			type == 0 && *(int*)(node + HEADLEN + i*moduleSize + 4) == ((DataI*)data)->x)
			break;
		if (type > 0)
		{
			memcpy(tempString, node + HEADLEN + i*moduleSize + 4, type);
			if (((DataS*)data)->x.compare(tempString) == 0) break;
		}
	}
	delete[] tempString;
	keyNum--;
	for (int j = i; j < keyNum; j++)
		memcpy(node + HEADLEN + j*moduleSize, node + HEADLEN + (j + 1)*moduleSize, moduleSize);
	if (root == *(int*)(node + 1) && keyNum == 1)
	{
		root = *(int*)(node + HEADLEN);
		*(int*)(buf + root*NODESIZE + 5) = -1;
		memset(node, 0, NODESIZE);
		saveToBuf(node);
		delete[] node;
	}
	else if (keyNum < capacity / 2)
	{
		bool flag;
		bool& ff = flag;
		char* nNode = new char[NODESIZE];
		Data* k = findBrother(node, nNode, ff);
		if (*(int*)(node + 9) + *(int*)(nNode + 9) < capacity)
		{
			if (ff)
			{
				char* temp;
				temp = nNode;
				nNode = node;
				node = temp;
				delete[] temp;
			}
			if (!node[0])
			{
				int& nNum = *(int*)(nNode + 9);
				if (type == -1) *(float*)(nNode + HEADLEN + nNum*moduleSize + 4) = ((DataF*)k)->x;
				if (type == 0) *(int*)(nNode + HEADLEN + nNum*moduleSize + 4) = ((DataI*)k)->x;
				if (type > 0) memcpy(nNode + HEADLEN + nNum*moduleSize + 4, &(((DataS*)k)->x), type * sizeof(char));
				for (i = 0; i < *(int*)(node + 9); i++)
					memcpy(nNode + HEADLEN + (nNum + 1 + i)*moduleSize, node + HEADLEN + i*moduleSize, moduleSize);
				nNum += *(int*)(node + 9) + 1;
				memcpy(nNode + HEADLEN + nNum*moduleSize, node + HEADLEN + *(int*)(node + 9)*moduleSize, 4);
			}
			else
			{
				for (i = 0; i < *(int*)(node + 9); i++)
					memcpy(nNode + HEADLEN + (*(int*)(nNode+9) + i)*moduleSize, node + HEADLEN + i*moduleSize, moduleSize);
				*(int*)(nNode + 9) += *(int*)(node + 9);
				memcpy(nNode + HEADLEN + *(int*)(nNode + 9)*moduleSize, node + HEADLEN + *(int*)(node + 9)*moduleSize, 4);
			}
			char* parent = new char[NODESIZE];
			memcpy(parent, buf + *(int*)(node + 5)*NODESIZE, NODESIZE);
			saveToBuf(node); saveToBuf(nNode);
			deleteEntry(parent, k);
			memset(node, 0, NODESIZE);
			saveToBuf(node); delete[] node; delete[] nNode;
		}
		else
		{
			if (!ff)
			{
				if (!node[0])
				{
					int& nnNum = *(int*)(nNode + 9);
					int pos = *(int*)(nNode + HEADLEN + nnNum*moduleSize);
					char* km = new char[10];
					memcpy(km, (nNode + HEADLEN + (nnNum - 1)*moduleSize + 4), moduleSize - 4);
					memset(nNode + HEADLEN + (nnNum - 1)*moduleSize + 4, 0, moduleSize);
					nnNum--;
					saveToBuf(nNode);
					delete[] nNode;
					*(int*)(node + HEADLEN + (*(int*)(node + 9) + 1)*moduleSize) = *(int*)(node + HEADLEN + *(int*)(node + 9)*moduleSize);
					for (i = 0; i < *(int*)(node + 9); i++)
						memcpy(node + HEADLEN + (i + 1)*moduleSize, node + HEADLEN + i*moduleSize, moduleSize);
					*(int*)(node + HEADLEN) = pos;
					if (type == -1) *(float*)(node + HEADLEN + 4) = ((DataF*)k)->x;
					if (type == 0) *(int*)(node + HEADLEN + 4) = ((DataI*)k)->x;
					if (type > 0) memcpy(node + HEADLEN + 4, &(((DataS*)k)->x), type * sizeof(char));
					saveToBuf(node);
					replaceInParent(k, node, km);
					delete[] node; delete[] km;
				}
				else
				{
					int& nnNum = *(int*)(nNode + 9);
					int pos = *(int*)(nNode + HEADLEN + nnNum*moduleSize);
					char* km = new char[10];
					char* temp = new char[moduleSize];
					memcpy(temp, nNode + HEADLEN + (nnNum - 1)*moduleSize, moduleSize);
					memcpy(km, (nNode + HEADLEN + (nnNum - 1)*moduleSize + 4), moduleSize - 4);
					memcpy(nNode + HEADLEN + (nnNum - 1)*moduleSize, nNode + HEADLEN + nnNum*moduleSize, 4);
					memset(nNode + HEADLEN + (nnNum - 1)*moduleSize + 4, 0, moduleSize);
					nnNum--;
					saveToBuf(nNode);
					delete[] nNode;
					*(int*)(node + HEADLEN + (*(int*)(node + 9) + 1)*moduleSize) = *(int*)(node + HEADLEN + *(int*)(node + 9)*moduleSize);
					for (i = 0; i < *(int*)(node + 9); i++)
						memcpy(node + HEADLEN + (i + 1)*moduleSize, node + HEADLEN + i*moduleSize, moduleSize);
					memcpy(node + HEADLEN, temp, moduleSize);
					saveToBuf(node);
					delete[] temp;
					replaceInParent(k, node, km);
					delete[] node; delete[] km;
				}
			}
			else
			{
				if (!nNode[0])
				{
					int& nNum = *(int*)(node + 9);
					int pos = *(int*)(node + HEADLEN + nNum*moduleSize);
					char* km = new char[10];
					memcpy(km, (node + HEADLEN + (nNum - 1)*moduleSize + 4), moduleSize - 4);
					memset(node + HEADLEN + (nNum - 1)*moduleSize + 4, 0, moduleSize);
					nNum--;
					saveToBuf(node);
					delete[] node;
					*(int*)(nNode + HEADLEN + (*(int*)(nNode + 9) + 1)*moduleSize) = *(int*)(nNode + HEADLEN + *(int*)(nNode + 9)*moduleSize);
					for (i = 0; i < *(int*)(nNode + 9); i++)
						memcpy(nNode + HEADLEN + (i + 1)*moduleSize, nNode + HEADLEN + i*moduleSize, moduleSize);
					*(int*)(nNode + HEADLEN) = pos;
					if (type == -1) *(float*)(nNode + HEADLEN + 4) = ((DataF*)k)->x;
					if (type == 0) *(int*)(nNode + HEADLEN + 4) = ((DataI*)k)->x;
					if (type > 0) memcpy(nNode + HEADLEN + 4, &(((DataS*)k)->x), type * sizeof(char));
					saveToBuf(nNode);
					replaceInParent(k, nNode, km);
					delete[] nNode; delete[] km;
				}
				else
				{
					int& nNum = *(int*)(node + 9);
					int pos = *(int*)(node + HEADLEN + nNum*moduleSize);
					char* km = new char[10];
					char* temp = new char[moduleSize];
					memcpy(temp, node + HEADLEN + (nNum - 1)*moduleSize, moduleSize);
					memcpy(km, (node + HEADLEN + (nNum - 1)*moduleSize + 4), moduleSize - 4);
					memcpy(node + HEADLEN + (nNum - 1)*moduleSize, node + HEADLEN + nNum*moduleSize, 4);
					memset(node + HEADLEN + (nNum - 1)*moduleSize + 4, 0, moduleSize);
					nNum--;
					saveToBuf(node);
					delete[] node;
					*(int*)(nNode + HEADLEN + (*(int*)(nNode + 9) + 1)*moduleSize) = *(int*)(nNode + HEADLEN + *(int*)(nNode + 9)*moduleSize);
					for (i = 0; i < *(int*)(nNode + 9); i++)
						memcpy(nNode + HEADLEN + (i + 1)*moduleSize, nNode + HEADLEN + i*moduleSize, moduleSize);
					memcpy(nNode + HEADLEN, temp, moduleSize);
					saveToBuf(nNode);
					delete[] temp;
					replaceInParent(k, node, km);
					delete[] nNode; delete[] km;
				}
			}
		}
	}
}

Data* BpTree::findBrother(char * node, char * dest, bool& flag)
{
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	int addr = *(int*)(node + 1);
	int fatherPos = *(int*)(node + 5);
	char* parent = new char[NODESIZE];
	memcpy(parent, buf + fatherPos*NODESIZE, NODESIZE);
	for (int i = 0; i < *(int*)(parent + 9); i++)
	{
		if (*(int*)(parent + HEADLEN + i*moduleSize) == addr)
			break;
	}
	int pos = (i == *(int*)(parent + 9) - 1) ? i - 1 : i + 1;
	flag = (i == *(int*)(parent + 9) - 1) ? 0 : 1;
	memcpy(dest, buf + pos*NODESIZE, NODESIZE);
	if (type == -1)
	{
		int tt = (i == *(int*)(parent + 9) - 1) ? i - 1 : i;
		DataF temp(*(float*)(parent + HEADLEN + tt*moduleSize + 4));
		delete[] parent;
		return &temp;
	}
	if (type == 0)
	{
		int tt = (i == *(int*)(parent + 9) - 1) ? i - 1 : i;
		DataI temp(*(int*)(parent + HEADLEN + tt*moduleSize + 4));
		delete[] parent;
		return &temp;
	}
	if (type > 0)
	{
		int tt = (i == *(int*)(parent + 9) - 1) ? i - 1 : i;
		char* tempS = new char[type];
		memcpy(tempS, parent + HEADLEN + tt*moduleSize + 4, type);
		DataS temp(tempS);
		delete[] parent; delete[] tempS;
		return &temp;
	}
}

void BpTree::replaceInParent(Data * k, char * node, char * newOne)
{
	size_t moduleSize = 4;
	int keyPos, i;
	if (type < 1) moduleSize += 4; else moduleSize += type;
	int addr = *(int*)(node + 1);
	int fatherPos = *(int*)(node + 5);
	char* parent = new char[NODESIZE];
	memcpy(parent, buf + fatherPos*NODESIZE, NODESIZE);
	char* tempString = new char[256];
	for (int i = 0; i < *(int*)(parent + 9); i++)
	{
		if (type == -1 && *(float*)(parent + HEADLEN + i*moduleSize + 4) == ((DataF*)k)->x ||
			type == 0 && *(int*)(parent + HEADLEN + i*moduleSize + 4) == ((DataI*)k)->x)
			break;
		if (type > 0)
		{
			memcpy(tempString, parent + HEADLEN + i*moduleSize + 4, type);
			if (((DataS*)k)->x.compare(tempString) == 0) break;
		}
	}
	delete[] tempString;
	memcpy(parent + HEADLEN + i*moduleSize + 4, newOne, moduleSize - 4);
	saveToBuf(parent);
	delete[] parent;
}