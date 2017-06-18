#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include"Basis.h"
#include"TableStruct.h"
#include"Catalog.h"
#include"API.h"

using namespace std;

Catalog() {
	cat.clear();
	cat_index.clear();
	fstream fileC,fileInd;
	fileC.open(catFile.c_str(),ios::in);
	fileInd.open(catIndFile.c_str(),ios::in);
	if(!fileC) {
		cout<<catFile<<"没有被创建"<<endl;
		fileC.open(catFile.c_str(),ios::out);
		cout<<catFile<<"创建成功"<<endl;
	} else {
		cout<<catFile<<"已经存在"<<endl;
		getline(fileC,bufCat);
		cout<<bufCat<<endl;
	}
	if(!fileInd) {
		cout<<catIndFile<<"没有被创建"<<endl;
		fileInd.open(catIndFile.c_str(),ios::out);
		cout<<catFile<<"创建成功"<<endl;
	} else {
		cout<<catIndFile<<"已经存在"<<endl;
		getline(fileInd,bufInd);
		cout<<bufInd<<endl;
	}
}

void addTable(TableStruct& tableinfo) {

}

bool hasTable(const string& tablename) {

}

void deleteTable(const string& tablename) {

}

TableStruct getTable(const string& tablename) {

}

void addIndex(const string& tablename, const string& indexname, const string& attriname) {

}

void deleteIndex(const string& indexname) {

}
