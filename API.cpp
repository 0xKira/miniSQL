#include <iostream>
#include "API.h"
using namespace std;
void API::createTable(TableStruct & table)
{
	extern Catalog cm;
	extern BufferManager bm;
	cm.addTable(table);
	bm.createTable(table.tableName);
}

void API::dropTable(string tableName)
{
	extern Catalog cm;
	extern BufferManager bm;
	cm.deleteTable(tableName);
	bm.dropTable(tableName);
}

void API::createIndex(string tableName, string indexName, string attrName)
{
	Catalog catalogManager;
	catalogManager.addIndex(tableName, indexName, attrName);
}

void API::dropIndex(string indexName)
{
	Catalog catalogManager;
	catalogManager.deleteIndex(indexName);
}

void API::select(string tableName, string attrName, vector<Condition>& condTable)
{
	extern RecordManager rm;
	
}

void API::insertData(string tableName, vector<Data>& data)
{
}

void API::deleteData(string tableName)
{
}

void API::deleteData(string tableName, vector<Condition>& condTable)
{
}