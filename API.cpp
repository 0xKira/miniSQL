#include <iostream>
#include "API.h"
using namespace std;
void API::createTable(TableStruct & table)
{
	Catalog catalogManager;
	catalogManager.addTable(table);
}

void API::dropTable(string tableName)
{
	Catalog catalogManager;
	catalogManager.deleteTable(tableName);
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
	// TODO:
	// wait for recordManager
	// print operation will be done in this section
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