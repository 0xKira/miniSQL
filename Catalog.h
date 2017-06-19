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
		void maptable();
		bool hasTable(const string& tablename);
		void addTable(TableStruct& table);
		void deleteTable(const string& tablename);
		TableStruct getTable(const string& tablename);
		void mapIndex();
		bool hasIndex(const string& indexname);
		void addIndex(const string& tablename, const string& indexname, const string& attriname);
		void deleteIndex(const string& indexname);

	public:
		string bufCat;
		string bufInd;
		map<string, int> cat;
		map<string, int> cat_index;
		string catFile="Cat.bin";
		string catIndFile="CatIndex.bin";
	private:
		bool InverttoInt(string s, int& x);
};

#endif /* Catalog_h */
