#ifndef Catalog_h
#define Catalog_h
#include<map>
#include<string>
#include"Basis.h"
#include"TableStruct.h"
using namespace std;

class Catalog {
	public:
		Catalog();
		~Catalog();
		void addTable(TableStruct& tableinfo);
		bool hasTable(const string& tablename);
		void deleteTable(const string& tablename);
		TableStruct getTable(const string& tablename);
		void addIndex(const string& tablename, const string& indexname, const string& attriname);
		void deleteIndex(const string& indexname);

	public:
		string bufCat;
		string bufInd;
		map<string, int> cat;
		map<string, int> cat_index;
		string catFile="Cat.bin";
		string catIndFile="CatIndex.bin";
};

#endif /* Catalog_h */
