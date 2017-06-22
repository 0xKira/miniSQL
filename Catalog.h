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
		void mapTable();
		bool hasTable(const string& tablename);
		void addTable(TableStruct& table);
		void deleteTable(const string& tablename);
		TableStruct &getTable(const string& tablename);
		void mapIndex();
		bool hasIndex(const string& indexname);
		void addIndex(const string& tablename, const string& indexname, const string& attriname);
		void deleteIndex(const string& indexname);
                bool dropTableIndex(const string& tablename);
	
	public:
		string bufCat;
		string bufInd;
		map<string, int> cat;
		map<string, int> cat_index;
		map<string, string> table_index;
		string catFile;
		string catIndFile;
	private:
		bool invertToInt(string s, int& x);
		//void Print_T(TableStruct &table);
		void writeback(TableStruct &table);
};

#endif /* Catalog_h */
