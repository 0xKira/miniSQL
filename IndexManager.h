#pragma once
#include <fstream>
#include <map>
#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"
#include "BufferUnit.h"
#include "Catalog.h"
#include "RecordManager.h"

extern Catalog cm;
extern RecordManager rm;

class IndexManager
{
public:
	map<string, string> names;
public:
	IndexManager() {};
	~IndexManager() {};
	void createIndex(const Attribute& attr, std::string indexName, TableStruct& table);

    void dropIndex(std::string indexName);
	int exactSearchWithIndex(std::string indexName, Data* value);
	vector<int> rangeSearchWithIndex(std::string indexName, Data* down, Data* up);
	void insertNew(string indexName, Data* value, int addr);
	void deleteOld(string indexName, Data* value);
};
