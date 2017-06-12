#include <iostream>
#include <fstream>
#include "IndexManager.h"
using namespace std;

void IndexManager::createIndex(const Attribute & attr, std::string indexName, const TableStruct & table)
{
	ofstream file;
	file.open("/data/" + table.tableName + ".index");

}