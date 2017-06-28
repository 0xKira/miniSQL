#ifndef Interpreter_h
#define Interpreter_h
#include<iostream>
#include<string>
#include<stdexcept>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include"Basis.h"
#include"TableStruct.h"
#include"Catalog.h"
#include"API.h"

extern Catalog cm;
extern API api;

class Interpreter {
//private:
public:
	std::string querys;//query string
public:
	Interpreter();
	~Interpreter();
	void inputQuery();
	void EXEC();
	void EXEC_CREATE();
	void EXEC_CREATE_TABLE();
	void EXEC_CREATE_INDEX();
	void EXEC_DROP();
	void EXEC_SELECT();
	void EXEC_INSERT();
	void EXEC_DELETE();
	void EXEC_PRINT(TableStruct &table);
	void EXEC_EXIT();
	void EXEC_FILE();

    vector<Condition> &ConditionList(TableStruct &table, string where);
	Tuple &TupleList(TableStruct& table, string where);
};

bool invertToInt(string s, int& x);
bool invertToFloat(string s, float& x);

#endif /* Interpreter_h */
