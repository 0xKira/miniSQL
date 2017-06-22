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

vector<Tuple> API::select(string tableName, vector<Condition>& condTable)
{
	extern RecordManager rm;
	extern Catalog cm;
	vector<Tuple> result;
	bool flag = rm.selectFromTable(cm.getTable(tableName), condTable, result);
	if (!flag) printf("Can't find");
	return result;
}

void API::insertData(string tableName, Tuple& tuple)
{
	extern RecordManager rm;
	extern Catalog cm;
	rm.insertIntoTable(tuple, cm.getTable(tableName));
}

void API::deleteData(string tableName, vector<Condition>& condTable)
{
	extern RecordManager rm;
	extern Catalog cm;
	rm.deleteFromTable(cm.getTable(tableName), condTable);
}