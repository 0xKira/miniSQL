#include<iostream>
#include<string>
#include<stdexcept>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include"Interpreter.h"
#include"Basis.h"
#include"TableStruct.h"
#include"Catalog.h"
#include"API.h"

using namespace std;

void Interpreter::inputQuery() {
	string temp;
	char c;
	int start=0;
	int valid=0;

	querys.clear();
	temp.clear();
	while(true) {
		c=cin.get();
		if((c<='z'&&c>='a')||(c<='Z'&&c>='A')||(c>='0'&&c<='9')) {
			temp += c;
			valid=1;
		} else if(c == '\n' || c == '\t' || c == ' ') {
			if(valid) {
				temp+=' ';
				valid=0;
			}
		} else if(c == '>' || c == '<' || c == '=') {
			if(valid) {
				temp += ' ';
				temp += c;
				temp += ' ';
				valid=0;
			} else {
				if(temp[temp.length() - 2] == '>'|| temp[temp.length() - 2] == '<'|| temp[temp.length() - 2] == '=') {
					temp[temp.length() - 1] = c;
					temp += ' ';
				} else {
					temp += c;
					temp += ' ';
				}
			}
		} else if(c == '*' || c == ',' || c == '(' || c == ')') {
			if(valid) {
				temp += ' ';
				temp += c;
				temp += ' ';
			} else {
				temp += c;
				temp += ' ';
			}
		} else if(c=='\'') {
			temp += ' ';
		} else if(c==';') {
			if(valid) {
				temp += ' ';
				temp += c;
			} else {
				temp += c;
			}
			break;
		} else {
			cout<<"wrong input"<<endl;
			while(cin.get() != ';') {
				;
			}
			temp.clear();
		}
	}

	while(temp[start]==' ') {
		start++;
	}
	querys = temp.substr(start, temp.length() - start);
	cout<<querys<<endl;
	return ;
}

void EXEC() {
	cout<<"minisql>>>";
	inputQuery();
	if(querys.substr(0,6)=="create") {
		EXEC_CREATE();
	} else if(querys.substr(0,4)=="drop") {
		EXEC_DROP();
	} else if(querys.substr(0,6)=="select") {
		EXEC_SELECT();
	} else if(querys.substr(0,6)=="insert") {
		EXEC_INSERT();
	} else if(querys.substr(0,6)=="delete") {
		EXEC_DELETE();
	} else if(querys.substr(0,4)=="exit") {
		EXEC_EXIT();
	} else if(querys.substr(0,10)=="show table") {
		EXEC_SHOW();
	} else if (query.substr(0, 8) == "execfile") {
		EXEC_FILE();
	} else {
		cout<<"error!"<<endl;
		//throw QueryException("ERROR: invalid query format!");
	}
}

void EXEC_CREATE() {
	if(querys[6]!=' ')
		//errror posotion
		cout<<"error!"<<endl;

	if(querys.substr(7, 5) == "table")
		EXEC_CREATE_TABLE();

	else if(querys.substr(7, 5) == "index")
		EXEC_CREATE_INDEX();
	else
		//error position
		cout<<"error!"<<endl;
}

void EXEC_CREATE_TABLE() {
	TableStruct table;
	int i;
	table.hasIndex=false;
	if(querys[12]!=' ')
		//errror posotion
		cout<<"error!"<<endl;

	string str=querys.substr(13,querys.length()-13);
	istringstream is(str);

	is>>table.tableName;
	cout<<table.tableName<<endl;
	string s;
	is>>s;
	if(s!="(")
		//errror posotion
		cout<<"error!"<<endl;
	//Attribute attr;
	while(is>>s) {
		if(s=="primary") {
			is>>s;
			if(s!="key")
				//errror posotion
				cout<<"error!"<<endl;
			table.hasIndex=true;
			is>>s;
			if(s!="(")
				//errror posotion
				cout<<"error!"<<endl;
			is>>s;
			for(i=0; i<table.attrs.size(); i++) {
				if(s==table.attrs[i].attrName) {
					table.attrs[i].isIndex=true;
					//增加index
					break;
				}
			}
			if(i>table.attrs.size())
				//errror posotion
				cout<<"error!"<<endl;
			is>>s;
			if(s!= ")")
				//errror posotion
				cout<<"error!"<<endl;
			is >> s;
			if (s != ")")
				//errror posotion
				cout<<"error!"<<endl;
			else
				break;
		} else {
			Attribute* temp = new Attribute;
			temp->attrName=s;
			is>>s;
			if(s=="int") {
				temp->type=0;
			} else if(s=="float") {
				temp->type=-1;
			} else if(s=="char") {
				is>>s;
				if(s!="(") {
					delete temp;
					//errror posotion
					cout<<"error 1!"<<endl;
				}

				is>>s;
				int num;
				if(InverttoInt(s,num)==false) {
					delete temp;
					//errror posotion
					cout<<"error 2!"<<endl;
				} else {
					temp->type=num;
				}

				is>>s;
				if(s!=")") {
					delete temp;
					//errror posotion
					cout<<"error 3!"<<endl;
				}
			}
			is>>s;
			if(s=="unique") {
				table.hasIndex =true;
				temp->isIndex=true;
				temp->unique=true;
				//设定index
				cout<<temp->attrName<<" is the index"<<endl;
				is>>s;
			} else {
				temp->isIndex=false;
				temp->unique=false;
			}

			if(s==",") {
				table.attrs.push_back(*temp);
			} else if(s==")") {
				table.attrs.push_back(*temp);
				break;
			} else {
				delete temp;
				//errror posotion
				cout<<"error 4!"<<endl;
			}
		}
	}
	return ;
}

void EXEC_CREATE_INDEX() {
	if(querys[12]!=' ')
		//errror posotion
		cout<<"error!"<<endl;

	string str=querys.substr(13,querys.length()-13);
	istringstream is(str);
	string s,indexname,tablename,attrname;

	is>>indexname;
	is>>s;
	if(s!="on") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>tablename;
	is>>s;
	if(s!="(") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>attrname;
	is>>s;
	if(s!=")") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>s;
	if(s!=";") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	//create index
	cout<<indexname<<tablename<<attrname<<endl;
}

void EXEC_DROP() {
	if(querys[4]!=' ')
		//errror posotion
		cout<<"error no space!"<<endl;
	Catalog ca;
	API ap;
	if(querys.substr(5, 5) == "table") {
		if(querys[10]!=' ') {
			//errror posotion
			cout<<"error no space2!"<<endl;
		}
		string str=querys.substr(11,querys.length()-11);
		istringstream is(str);
		string tablename;
		is>>tablename;
		if(!hasTable(tablename)) {
			//errror posotion
			cout<<"error no such table!"<<endl;
		}
		string s;
		is>>s;
		if(s!=";") {
			//errror posotion
			cout<<"error!"<<endl;
		}
		ap.dropTable(tablename);
		cout<<"drop the table named "<<tablename<<endl;
		//DROP TABLE BY OTHERS
	} else if(querys.substr(5, 5) == "index") {
		if(querys[10]!=' ') {
			//errror posotion
			cout<<"error!"<<endl;
		}
		//drop index stunameidx;
		string str=querys.substr(11,querys.length()-11);
		istringstream is(str);
		string indexname;
		is>>indexname;
		string s;
		is>>s;
		if(s!=";") {
			//errror posotion
			cout<<"error!"<<endl;
		}
		ap.dropIndex(indexname);
		cout<<"drop the index named "<<indexname<<endl;
		//DROP index BY OTHERS
	} else
		//error position
		cout<<"error!"<<endl;
}

vector<Condition> ConditionList(TableStruct& table, string where) {
	vector<Condition> cond;
	cond.clear();
	istringstream is(where);
	string attrname,op,s;
	int i;
	int i_type;
	float f_type;

	while(true) {
		is>>attrname;
		//cout<<attername<<endl;
		Condition* temp = new Condition;
		for(i=0; i<table->attrs.size(); i++) {
			if(table->attrs[i].attrName==attrname) {
				temp->attrindex=i;
				break;
			}
		}
		if(i==table->attrs.size()) {
			delete temp;
			//errror posotion
			cout<<"error! no such attribute"<<endl;
		}
		is>>op;
		//cout<<op<<endl;
		if(op=="=") {
			temp->flag=EQ;
		} else if(op=="<=") {
			temp->flag=LEQ;
		} else if(op==">=") {
			temp->flag=GEQ;
		} else if(op=="<") {
			temp->flag=L;
		} else if(op==">") {
			temp->flag=G;
		} else if(op=="<>") {
			temp->flag=NEQ;
		} else {
			delete temp;
			//errror posotion
			cout<<"error! no such operation"<<endl;
		}
		is>>s;
		if(s=="and") {
			delete temp;
			//errror posotion
			cout<<"error! no such operation"<<endl;
		}
		if((table->attrs[temp->attrindex].type<0)&&(InverttoToFloat(s, f_type))) {
			DataF data_f(f_type);
			temp->d=data_f;
			cond.push_back(*temp);
		} else if((table->attrs[temp->attrindex].type==0)&&(InverttoInt(s, i_type))) {
			DataI data_i(i_type);
			temp->d=data_i;
			cond.push_back(*temp);
		} else if(table->attrs[temp->attrindex].type>0) {
			DataS data_s(s);
			temp->d=data_s;
			cond.push_back(*temp);
		} else {
			delete temp;
			//errror posotion
			cout<<"error! no such kind of type"<<endl;
		}
		is>>s;
		if(s==";")
			break;
		else if(s!="and") {
			//errror posotion
			cout<<"error! no such SQL"<<endl;
		}

	}

	return cond;
}

void EXEC_SELECT() {
	if(querys[6]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	Catalog ca;
	API ap;
	TableStruct table;
	string str=querys.substr(7,querys.length()-7);
	istringstream is(str);
	string s,tablename,where;
	int start;

	is>>s;
	if(s!="*") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>s;
	if(s!="from") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>tablename;
	if(!ca.hasTable(tablename)) {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>s;
	if(s==";") {
		//select all
		table=ap.Select(tablename,,);
		//print all the tuples;
		return ;
	} else if(s!="where") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	for(start=7; start<(querys.length()-5); start++) {
		if(querys.substr(start,5)=="where")
			break;
	}
	where=querys.substr(start+6,querys.length()-6-start);

	table=ca.getTable(tablename);
	vector<Condition> cond;
	cond.clear();
	cond=ConditionList(table,where);
	/*TableStruct output=ap.select(tableName, cond);
	EXEC_PRINT(output);*/

	return ;
}

void EXEC_INSERT() {
	if(querys[6]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	string str=querys.substr(7,querys.length()-7);
	istringstream is(str);
	string s,tablename;
	Catalog ca;
	TableStruct table;

	is>>s;
	if(s!="into") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>tablename;
	if(!ca.hasTable(tablename)) {
		//errror posotion
		cout<<"error!"<<endl;
	}
	table=ca.getTable(tablename);
	is>>s;
	if(s!="values") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	/*is>>s;
	if(s!="(")
	{
		//errror posotion
		cout<<"error!"<<endl;
	}
	Tuple onetuple;
	while(true)
	{

	}*/
	API ap;
	ap.insertData(tablename,onetuple);
}

void EXEC_DELETE() {
	if(querys[6]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	Catalog ca;
	API ap;
	TableStruct table;
	string str=querys.substr(7,querys.length()-7);
	istringstream is(str);
	string s,tablename,where;
	int start;

	is>>s;
	if(s!="from") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>tablename;
	if(!ca.hasTable(tablename)) {
		//errror posotion
		cout<<"error!"<<endl;
	}
	is>>s;
	if(s==";") {
		//select all
		table=ap.deleteData(tablename);
		//print all the tuples;
		return ;
	} else if(s!="where") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	for(start=7; start<(querys.length()-5); start++) {
		if(querys.substr(start,5)=="where")
			break;
	}
	where=querys.substr(start+6,querys.length()-6-start);

	table=ca.getTable(tablename);
	vector<Condition> cond;
	cond.clear();
	cond=ConditionList(table,where);
	/*TableStruct output=ap.select(tableName, cond);
	EXEC_PRINT(output);*/

	return ;
}

void EXEC_PRINT(TableStruct &table) {
	int i;

	for(i=0; i<table.attrs.size(); i++) {
		cout<<table.attrs[i].attrName<<" "<<table.attrs[i].attrName<<endl;
	}
}

void EXEC_EXIT() {

}

void EXEC_FILE() {
	if(querys[8]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	string str=querys.substr(9,querys.length()-9);
	istringstream is(str);
	string filename;
	is>>filename;
	cout<<"we will open the file named: "<<filename<<endl;
}

bool InverttoInt(string s, int& x) {
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

bool InvertToFloat(string s, float& x) {
	x=0;
	int int_f=0;
	float float_f=0;
	int i;
	int pdot;

	for(pdot=0; pdot<s.length(); pdot++)
		if(s[pdot]=='.')
			break;
	for(i=0; i<pdot; i++) {
		if(s[i]<='9' && s[i]>='0')
			int_f = int_f * 10 + s[i] - '0';
		else
			return false;
	}
	x+=int_f;
	for(i=s.length()-1; i>pdot; i--) {
		if(s[i]<='9' && s[i]>='0')
			float_f = (float_f+s[i] - '0')/10.0;
		else
			return false;
	}
	x+=float_f;

	return true;
}
