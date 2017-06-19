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
		is>>s;
		if(s=="1") {
			cout<<"map this table named "<<tablename<<endl;
			cat.insert(pair<string,int>(tablename,position));
		}
	}
}

bool Catalog::hasTable(const string& tablename) {
	int pos;

	if(cat.find(tablename)!=cat.end())
		pos=cat[tablename];
	else
		return false;
	cout<<pos<<endl;
	string str=bufCat.substr(pos,bufCat.length()-pos);
	istringstream is(str);
	string s;

	is>>s;
	is>>s;
	is>>s;
	is>>s;
	//cout<<"the valid of this table is "<<s<<endl;
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
		return ;
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
	cout<<"add this table successfully"<<endl;
}

void Catalog::deleteTable(const string& tablename) {
	if(!hasTable(tablename)) {
		//error condtion
		cout<<"this table do not exit!"<<endl;
		return ;
	}

	int pos=cat[tablename];
	string str=bufCat.substr(pos,bufCat.length()-pos);
	istringstream is(str);
	string s;
	int position=pos;

	is>>s;
	position=position+s.size()+1;
	is>>s;
	position=position+s.size()+1;
	is>>s;
	position=position+s.size()+1;
	cout<<"before:the valid of this table is "<<bufCat[position]<<endl;
	bufCat[position]='0';
	cout<<"after:the valid of this table is "<<bufCat[position]<<endl;
	cat.erase(tablename);
	cout<<"delete this table successfully"<<endl;
}

void Catalog::Print_T(TableStruct& table) {
	int i;

	cout<<"show this table: "<<table.tableName<<" "<<table.hasIndex<<endl;

	for(i=0; i<table.attrs.size(); i++) {
		cout<<table.attrs[i].attrName<<' '<<table.attrs[i].type<<' ';
		cout<<table.attrs[i].unique<<' '<<table.attrs[i].isIndex<<endl;
	}
}

TableStruct Catalog::getTable(const string& tablename) {
	if(!hasTable(tablename)) {
		//error condition
		cout<<"error! don't exit this table"<<endl;
	}
	TableStruct table;
	int pos=cat[tablename];
	string str=bufCat.substr(pos,bufCat.length()-pos);
	istringstream is(str);
	string s;
	int i,num;

	is>>s;
	is>>s;
	is>>table.tableName;
	is>>s;
	is>>s;
	InverttoInt(s,i);
	for(; i>0; i--) {
		Attribute* temp = new Attribute;
		is>>temp->attrName;
		is>>s;
		if(s=="-1")
			temp->type=-1;
		else {
			InverttoInt(s,num);
			temp->type=num;
		}
		is>>s;
		InverttoInt(s,num);
		temp->unique=num;
		is>>s;
		InverttoInt(s,num);
		temp->isIndex=num;
		table.attrs.push_back(*temp);
	}
	Print_T(table);

	return table;
}

void Catalog::mapIndex() {
	string str;
	istringstream is;
	string s,indexname;
	int pos=0,length,position;

	while(pos<bufInd.length()) {
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
	    is>>indexname;
	    is>>s;
		if(s=="1") {
			cout<<"map this index named "<<indexname<<endl;
			cat_index.insert(pair<string,int>(indexname,position));
		}
   }
}

bool Catalog::hasIndex(const string& indexname) {
	int pos;
	
	if(cat_index.find(indexname)!=cat_index.end())
		pos=cat_index[indexname];
	else
		return false;

    string str=bufInd.substr(pos,bufInd.length()-pos);
	istringstream is(str);
	string s;
	
	is>>s;
	is>>s;
	is>>s;
	if(s!=indexname)
	{
		//errror condition
		return false;
	}
	is>>s;
	//cout<<"the valid of this table is "<<s<<endl;
	if(s=="1") {
		return true;
	} else if(s=="0")
		return false;
	else {
		//errror condition
		return false;
	}
}

void Catalog::addIndex(const string& tablename, const string& indexname, const string& attriname) {
	ostringstream os,os2;
	int pos1,epos;
    int i;
    
	if(hasIndex(indexname)) {
		//error condition
		cout<<"this index has already exit"<<endl;
		return ;
	}
	TableStruct table;
	table=getTable(tablename);
	for(i=0; i<table.attrs.size(); i++) {
		if(attriname==table.attrs[i].attrName) {
			if(table.attrs[i].unique)
			{
				if(table.attrs[i].isIndex)
				{
					cout<<"there is a index in this attributr"<<endl;
					return ;
				}
				else
				{
					//add the index
					break;
				}
			}
			else
			{
				cout<<"this attributr is not unique"<<endl;
				return ;
			}
		}
	}
	if(i>=table.attrs.size())
	{
		//errror posotion
		cout<<"error!"<<endl;
		return ;
	}		
	pos1=bufInd.size();
	os<<indexname<<' '<<true<<' '<<tablename<<' '<<attriname<<';';
	epos=os.str().size();
	os2<<pos1<<' '<<epos<<' '<<os.str();
	bufInd+=os2.str();
	cat_index.insert(pair<string,int>(indexname,pos1));
	cout<<"add this index successfully"<<endl;
}

void Catalog::deleteIndex(const string& indexname) {
    
}
