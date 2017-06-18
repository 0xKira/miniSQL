#include <iostream>
#include <fstream>
#include "IndexManager.h"
#include "BpTree.h"
using namespace std;

void IndexManager::createIndex(const Attribute & attr, std::string indexName, const TableStruct & table)
{	
	BpTree index(table, indexName, attr);
	
}