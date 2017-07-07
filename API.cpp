#include <iostream>
#include "API.h"

using namespace std;

void API::createTable(TableStruct &table) {
    cm.addTable(table);
    bm.createTable(table.tableName);
}

void API::dropTable(string tableName) {
    cm.deleteTable(tableName);
    bm.dropTable(tableName);
}

void API::createIndex(string tableName, string indexName, string attrName) {
    cm.addIndex(tableName, indexName, attrName);
}

void API::dropIndex(string indexName) {
    cm.deleteIndex(indexName);
}

vector<Tuple> &API::select(TableStruct &table, vector<Condition> &condTable) {
    vector<Tuple> *result = new vector<Tuple>;
    bool flag;
    if (table.hasIndex) {
		if (condTable.size() == 1 && condTable[0].flag == EQ)
		{
			int temp = im.exactSearchWithIndex(im.names[table.tableName], condTable[0].d);
			vector<int> tempInt;
			tempInt.push_back(temp);
			rm.selectFromTableWithIndex(table, condTable, tempInt, *result);
		}
		else
		{
			vector<int> tempInt;
			tempInt = im.rangeSearchWithIndex(im.names[table.tableName], condTable[0].d, condTable[condTable.size()].d);
			rm.selectFromTableWithIndex(table, condTable, tempInt, *result);
		}
    } else {
        flag = rm.selectFromTable(table, condTable, *result);
    }
    if (!flag) printf("Can't find\n");
    return *result;
}

void API::insertData(TableStruct &table, Tuple &tuple) {
    rm.insertIntoTable(table, tuple);
}

void API::deleteData(TableStruct &table, vector<Condition> &condTable) {
    if (table.hasIndex) {
		for (int i = 0; i < condTable.size(); i++)
		{
			im.deleteOld(im.names[table.tableName], condTable[0].d);
		}
    } else {
        rm.deleteFromTable(table, condTable);
    }
}