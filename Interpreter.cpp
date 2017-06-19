#include<iostream>
#include<string>
#include<stdexcept>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include"Interpreter.h"

using namespace std;

Interpreter::Interpreter() {
	querys.clear();
}

Interpreter::~Interpreter() {
}

void Interpreter::inputQuery() {
	string temp;
	char c;
	int start=0;
	int valid=0;

	querys.clear();
	temp.clear();
	while(true) {
		c=cin.get();
		if((c<='z'&&c>='a')||(c<='Z'&&c>='A')||(c>='0'&&c<='9')||c=='_'||c=='.') {
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

void Interpreter::EXEC() {
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
	} else if (querys.substr(0, 8) == "execfile") {
		EXEC_FILE();
	} else {
		cout<<"error!"<<endl;
		//throw QueryException("ERROR: invalid query format!");
	}
}

void Interpreter::EXEC_CREATE() {
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

void Interpreter::EXEC_CREATE_TABLE() {
	if(querys[12]!=' ')
		//errror posotion
		cout<<"error!"<<endl;

	Catalog ca;
	API ap;
	string str=querys.substr(13,querys.length()-13);
	istringstream is(str);
	string s,tablename;
	vector<Attribute> attrs;
	size_t tuplenum;
	bool hasIndex;
	int i;

	is>>tablename;
	hasIndex=false;
	tuplenum=0;
	//cout<<table.tableName<<endl;
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
			hasIndex=true;
			is>>s;
			if(s!="(")
				//errror posotion
				cout<<"error!"<<endl;
			is>>s;
			for(i=0; i<attrs.size(); i++) {
				if(s==attrs[i].attrName) {
					hasIndex=true;
					attrs[i].isIndex=true;
					attrs[i].unique=true;
					ca.addIndex(tablename,tablename+attrs[i].attrName,attrs[i].attrName);
					//ap.createIndex(tablename,tablename+attrs[i].attrName,attrs[i].attrName);
					break;
				}
			}
			if(i>attrs.size())
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
				if(!invertToInt(s,num)) {
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
				temp->isIndex=false;
				temp->unique=true;
				is>>s;
			} else {
				temp->isIndex=false;
				temp->unique=false;
			}

			if(s==",") {
				attrs.push_back(*temp);
			} else if(s==")") {
				attrs.push_back(*temp);
				break;
			} else {
				delete temp;
				//errror posotion
				cout<<"error 4!"<<endl;
			}
		}
	}
	TableStruct *table = new TableStruct(tablename,attrs,hasIndex,tuplenum);
	ca.addTable(*table);
	//ap.createTable(*table);
	cout<<"Interpreter create table successfully"<<endl;

	return ;
}

void Interpreter::EXEC_CREATE_INDEX() {
	if(querys[12]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	Catalog ca;
	API ap;
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
	ca.addIndex(tablename,indexname,attrname);
	//ap.createIndex(tablename,indexname,attrname);

	cout<<"Interpreter create index successfully"<<endl;
}

void Interpreter::EXEC_DROP() {
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
		string s;
		is>>s;
		if(s!=";") {
			//errror posotion
			cout<<"error!"<<endl;
		}
		ca.deleteTable(tablename);
		//ap.dropTable(tablename);
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
		//ap.dropIndex(indexname);
		ca.deleteIndex(indexname);
		cout<<"drop the index named "<<indexname<<endl;
		//DROP index BY OTHERS
	} else
		//error position
		cout<<"error!"<<endl;
}

vector<Condition> Interpreter::ConditionList(TableStruct& table, string where) {
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
		for(i=0; i<table.attrs.size(); i++) {
			if(table.attrs[i].attrName==attrname) {
				temp->attrIndex=i;
				break;
			}
		}
		if(i==table.attrs.size()) {
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
		if((table.attrs[temp->attrIndex].type<0)&&(invertToFloat(s, f_type))) {
			Data* data_f = new DataF(f_type);
			temp->d=data_f;
			cond.push_back(*temp);
		} else if((table.attrs[temp->attrIndex].type==0)&&(invertToInt(s, i_type))) {
			Data* data_i = new DataI(i_type);
			temp->d=data_i;
			cond.push_back(*temp);
		} else if(table.attrs[temp->attrIndex].type>0) {
			Data* data_s = new DataS(s);
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

void Interpreter::EXEC_SELECT() {
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
		//table=ap.Select(tablename,,);
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

Tuple Interpreter::TupleList(TableStruct& table, string where) {
	Tuple tup;
	tup.data.clear();
	istringstream is(where);
	string s;
	int i_type;
	float f_type;
	int i;

	is>>s;
	if(s!="(") {
		//errror posotion
		cout<<"error! wrong SQL of values"<<endl;
	}
	for(i=0; i<table.attrs.size(); i++) {
		is>>s;
		if(is==")") {
			//errror posotion
			cout<<"error! wrong SQL of values"<<endl;
		}
		if((table.attrs[i].type<0)&&(invertToFloat(s, f_type))) {
			Data* da = new DataF(f_type);
			tup.data.push_back(da);
		} else if((table.attrs[i].type<0)&&(invertToInt(s, i_type))) {
			Data* da = new DataI(i_type);
		} else if(table.attrs[i].type<0) {
			Data* da = new DataS(s);
		} else {
			//errror posotion
			cout<<"error! "<<endl;
		}
	}
	is>>s;
	if(s==")")
		return tup;
	else {
		//errror posotion
		cout<<"error! wrong SQL of values"<<endl;
	}
}

void Interpreter::EXEC_INSERT() {
	if(querys[6]!=' ')
		//errror posotion
		cout<<"error!"<<endl;

	string str=querys.substr(7,querys.length()-7);
	istringstream is(str);
	string s,tablename,values;
	Catalog ca;
	Tuple onetuple;
	API ap;
	TableStruct table;
	int start;

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
	is>>s;
	if(s!="values") {
		//errror posotion
		cout<<"error!"<<endl;
	}
	for(start=7; start<(querys.length()-5); start++) {
		if(querys.substr(start,6)=="values")
			break;
	}
	values=querys.substr(start+7,querys.length()-7-start);

	table=ca.getTable(tablename);
	onetuple.data.clear();
	onetuple=TupleList(table,values);
	//ap.insertData(tablename,onetuple.data);

}

void Interpreter::EXEC_DELETE() {
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
		//table=ap.deleteData(tablename);
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

void Interpreter::EXEC_PRINT(TableStruct &table) {
	int i;

	for(i=0; i<table.attrs.size(); i++) {
		cout<<table.attrs[i].attrName<<" "<<table.attrs[i].attrName<<endl;
	}
}

void Interpreter::EXEC_EXIT() {
	cout<<"exit this minisql"<<endl;
}

void Interpreter::EXEC_FILE() {
	if(querys[8]!=' ')
		//errror posotion
		cout<<"error!"<<endl;
	string str=querys.substr(9,querys.length()-9);
	istringstream is(str);
	string filename;
	string temp;
	char c;
	int valid=0;
	bool end=false;
	int start;

	is>>filename;
	cout<<filename<<endl;
	ifstream in(filename.c_str());
	if(!in.is_open()) {
		cout<<"can not open this file: "<<filename<<endl;
	}
	while(!in.eof()) {
		querys.clear();
		end=false;
		while(true) {
			c=in.get();
			if(c==EOF) {
				end=true;
				break;
			}

			if((c<='z'&&c>='a')||(c<='Z'&&c>='A')||(c>='0'&&c<='9')||c=='_'||c=='.') {
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
		if(end)
			break;
		while(temp[start]==' ') {
			start++;
		}
		querys = temp.substr(start, temp.length() - start);

		cout<<"minisql>>>"<<querys<<endl;
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
		} else if (querys.substr(0, 8) == "execfile") {
			EXEC_FILE();
		} else {
			cout<<"error!"<<endl;
			//throw QueryException("ERROR: invalid query format!");
		}
	}
}

bool invertToInt(string s, int& x) {
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

bool invertToFloat(string s, float& x) {
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
