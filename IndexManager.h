#pragma once
#include <fstream>
#include "Basis.h"
#include "TableStruct.h"
#include "BufferManager.h"
#include "BufferUnit.h"

class IndexManager
{
	IndexManager() {};
	~IndexManager() {};
	void createIndex(const Attribute& attr, std::string indexName, const TableStruct& table);
	void dropIndex(std::string indexName, const TableStruct& table);
	vector<Tuple> exactSearchWithIndex(std::string tableName, std::string indexName, const Data& value);
	vector<Tuple> rangeSearchWithIndex(std::string tableName, std::string indexName, const Data& down, const Data& up);
	void insertNew(const Data& value);
	void deleteOld(const Data& value);
};
