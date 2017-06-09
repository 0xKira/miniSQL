#ifndef Catalog_h
#define Catalog_h
#include "base.h"
using namespace std;

class Catalog{ 
public:
	Catalog();
	~Catalog();
	void addTable(TableStruct& tableinfo);
	bool hasTable(const string& tablename);
	void deleteTable(const string& tablename);
	TableStruct getTable(const string& tablename);
	void addIndex(const string& tablename, const string& indexname, const string& attriname);
	void deleteIndex(const string& indexname);
};

#endif /* Catalog_h */
