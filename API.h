#pragma once
#include "Basis.h"
#include "TableStruct.h"
#include "Catalog.h"
#include "BufferManager.h"
#include "IndexManager.h"

class API
{
public:
	API() {};
	~API() {};
	void createTable(TableStruct&);
	void dropTable(std::string);
	void createIndex(std::string);
	void dropIndex(std::string);
	void select(const Attribute& attr, std::string tableName, vector<Condition>& condTable);
	void insertData(std::string tableName, vector<Data>& data);
	void deleteData(std::string tableName);
	void deleteData(std::string tableName, vector<Condition>& condTable);
};