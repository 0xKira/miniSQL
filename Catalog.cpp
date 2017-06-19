#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include"Basis.h"
#include"TableStruct.h"
#include"Catalog.h"

using namespace std;

Catalog::Catalog() {
	cat.clear();
	cat_index.clear();
	catFile="Cat.bin";
	catIndFile="CatIndex.bin";
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
		mapTable();
	}
	if(!fileInd) {
		cout<<catIndFile<<"没有被创建"<<endl;
		fileInd.open(catIndFile.c_str(),ios::out);
		cout<<catFile<<"创建成功"<<endl;
	} else {
		cout<<catIndFile<<"已经存在"<<endl;
		getline(fileInd,bufInd);
	}
}

Catalog::~Catalog() {
	fstream fileC,fileInd;

	fileC.open(catFile.c_str(),ios::out);
	fileInd.open(catIndFile.c_str(),ios::out);
	if(!fileC) {
		cout<<catFile<<"can not open"<<endl;
	} else {
		cout<<catFile<<"open"<<endl;
		fileC<<bufCat<<endl;
	}
	if(!fileInd) {
		cout<<catIndFile<<"can not open"<<endl;
	} else {
		cout<<catIndFile<<"open"<<endl;
		fileInd<<bufInd<<endl;
	}
	bufCat.clear();
	bufInd.clear();
	cat.clear();
	cat_index.clear();
}

bool Catalog::InverttoInt(string s, int& x) {
	int i;
	x = 0;
	for(i=0; i<s.length(); i++) {
		if(s[i]<='9' && s[i]>='0')
			x = x * 10 + s[i] - '0';
		else
			return false;
	}
	return true;
}

void Catalog::mapTable() {
	string str;
	istringstream is;
	string s,tablename;
	int pos=0,length,position;

	while(pos<bufCat.length()) {
		str=bufCat.substr(pos,bufCat.length()-pos);
		is.str(str);
		is>>s;
		if(!InverttoInt(s,position)) {
			//errror condition
			return ;
		}
		pos=position+s.size();
		is>>s;
		if(!InverttoInt(s,length)) {
			//errror condition
			return ;
		}
		pos=pos+length+s.size()+2;
		is>>tablename;
		cat.insert(pair<string,int>(tablename,position));
	}
}

bool Catalog::hasTable(const string& tablename) {
	int pos;

	if(cat.find(tablename)!=cat.end())
		pos=cat[tablename];
	else
		return false;
	string str=bufCat.substr(pos,bufCat.length()-pos);
	istringstream is(str);
	string s;

	is>>s;
	cout<<"the posotion of this table is "<<s<<endl;
	is>>s;
	cout<<"the length of this table is "<<s<<endl;
	is>>s;
	cout<<"the name of this table is "<<s<<endl;
	is>>s;
	cout<<"the valid of this table is "<<s<<endl;
	if(s=="1") {
		return true;
	} else if(s=="0")
		return false;
	else {
		//errror condition
		return false;
	}
}

void Catalog::addTable(TableStruct& table) {
	ostringstream os,os2;
	int pos1,epos;

	if(hasTable(table.tableName)) {
		//error condtion
		cout<<"this table has already exit!"<<endl;
	}
	os.clear();
	os2.clear();
	os<<table.tableName;
	os<<' ';
	os<<true;
	os<<' ';
	os<<table.attrs.size();
	os<<' ';
	for(int i=0; i<table.attrs.size(); i++) {
		os<<table.attrs[i].attrName;
		os<<' ';
		os<<table.attrs[i].type;
		os<<' ';
		os<<table.attrs[i].unique;
		os<<' ';
		os<<table.attrs[i].isIndex;
		os<<' ';
	}
	os<<table.hasIndex;
	os<<' ';
	os<<table.tupleNum;
	os<<' ';
	os<<table.tupleSize;
	os<<';';
	pos1=bufCat.size();
	epos=os.str().size();
	os2<<pos1<<' '<<epos<<' '<<os.str();
	bufCat+=os2.str();
	cat.insert(pair<string,int>(table.tableName,pos1));
}

void Catalog::deleteTable(const string& tablename) {

}

TableStruct Catalog::getTable(const string& tablename) {

}

void Catalog::addIndex(const string& tablename, const string& indexname, const string& attriname) {

}

void Catalog::mapIndex() {

}

void Catalog::deleteIndex(const string& indexname) {

}
