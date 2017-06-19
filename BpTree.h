#include <iostream>
#include <string>
#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"

using namespace std;
class BpTree
{
public:
	int size, type;
	string tableName, indexName;
	char* buf;
public:
	BpTree(const TableStruct& table, const string& index, const Attribute& attr);
	BpTree(const string& index);
	~BpTree();
	void initialize(Data* data);
	int find(Data* data);
	void insert(Data* data, int pos);
};