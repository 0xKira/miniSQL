#include "BpTree.h"
#include "RecordManager.h"
#include <fstream>
using namespace std;
#define NODESIZE 1024
#define HEADLEN 17

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
		indexFile << attr.type << endl;
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
		indexFile >> type;
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
	indexFile << type << endl;
	indexFile << buf << endl;
	indexFile.close;
}

void BpTree::initialize(Data* data)
{
	buf = (char*)malloc(sizeof(char)*NODESIZE);
	size++;
	buf[0] = 1;				//leaf node
	*(int*)(buf + 1) = 0;	//node pos
	*(int*)(buf + 5) = -1;	//father pos
	*(int*)(buf + 9) = 1;	//key number
	*(int*)(buf + 13) = 0;	//pos in table or child pos
	if (type == -1)
		*(int*)(buf + HEADLEN) = ((DataF*)data)->x;
	else if (type == 0)
		*(int*)(buf + HEADLEN) = ((DataI*)data)->x;
	else
		memcpy(buf + HEADLEN, &(((DataS*)data)->x), type*sizeof(char));
}

int BpTree::find(Data * data)
{
	char* currentNode = new char[NODESIZE];
	memcpy(currentNode, buf, NODESIZE);
	bool ifLeaf = (bool)buf[0];
	int keyNum = *(int*)(buf + 9);
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
		ifLeaf = (bool)buf[0];
		keyNum = *(int*)(buf + 9);
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

void BpTree::insert(Data * data, int pos)
{
	if (size == 0)
		initialize(data);
	else
	{
		size++;

	}
}