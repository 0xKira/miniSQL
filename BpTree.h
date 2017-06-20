#include <iostream>
#include <string>
#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"

using namespace std;
class BpTree
{
public:
	int size, type, root;
	string tableName, indexName;
	char* buf;
public:
	BpTree(const TableStruct& table, const string& index, const Attribute& attr);
	BpTree(const string& index);
	~BpTree();
	void initialize(Data* data);
	int find(Data* data);
	void insert(Data* data, int pos);
	void insertInParent(char* node, float data, char* newNode);
	void insertInParent(char* node, int data, char* newNode);
	void insertInParent(char* node, char* data, char* newNode, int length);
	void saveToBuf(char* node);
	void deleteData(Data* data);
	void deleteEntry(char* node, Data* data);
	Data* findBrother(char* node, char* dest, bool& flag);
};