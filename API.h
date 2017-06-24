#pragma once
#include "Basis.h"
#include "TableStruct.h"
#include "Catalog.h"
#include "BufferManager.h"
#include "IndexManager.h"
#include "RecordManager.h"

extern Catalog cm;
extern BufferManager bm;
extern RecordManager rm;

class API
{
public:
	API() {};
	~API() {};
	void createTable(TableStruct&);
	void dropTable(std::string);
	void createIndex(std::string, std::string, std::string);
	void dropIndex(std::string);

	vector<Tuple> &select(TableStruct &table, vector<Condition> &condTable);
	void insertData(TableStruct &table, Tuple& data);
	//void deleteData(std::string tableName);
	void deleteData(TableStruct &table, vector<Condition> &condTable);
};