#include <iostream>
#include "API.h"
using namespace std;
void API::createTable(TableStruct & table)
{
	cm.addTable(table);
	bm.createTable(table.tableName);
}

void API::dropTable(string tableName)
{
	cm.deleteTable(tableName);
	bm.dropTable(tableName);
}

void API::createIndex(string tableName, string indexName, string attrName)
{
	cm.addIndex(tableName, indexName, attrName);
}

void API::dropIndex(string indexName)
{
    cm.deleteIndex(indexName);
}

vector<Tuple> API::select(string tableName, vector<Condition>& condTable)
{
	vector<Tuple> result;
	bool flag = rm.selectFromTable(cm.getTable(tableName), condTable, result);
	if (!flag) printf("Can't find");
	return result;
}

void API::insertData(TableStruct &table, Tuple& tuple)
{
    rm.insertIntoTable(table, tuple);
}

void API::deleteData(string tableName, vector<Condition>& condTable)
{
	rm.deleteFromTable(cm.getTable(tableName), condTable);
}