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

void EXEC()
{
	cout<<"minisql>>>";
	inputQuery();
	if(querys.substr(0,6)=="create")
	{
		EXEC_CREATE();
	}
	else if(querys.substr(0,4)=="drop")
	{
		EXEC_DROP();
	}
	else if(querys.substr(0,6)=="select")
	{
		EXEC_SELECT();
	}
	else if(querys.substr(0,6)=="insert")
	{
		EXEC_INSERT();
	}
	else if(querys.substr(0,6)=="delete")
	{
		EXEC_DELETE();
	}
	else if(querys.substr(0,4)=="exit")
	{
		EXEC_EXIT();
	}
	else if(querys.substr(0,10)=="show table")
	{
        EXEC_SHOW();
    }
    else if (query.substr(0, 8) == "execfile")
	{
			EXEC_FILE();
    }
    else
    {
    	cout<<"error!"<<endl;
    	//throw QueryException("ERROR: invalid query format!");
	}
}

void EXEC_CREATE()
{
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

void EXEC_CREATE_TABLE()
{
	TableStruct table;
	table.hasIndex=false;
	if(querys[12]!=' ')
	    //errror posotion
	    cout<<"error!"<<endl;
	    
	string str=querys.substr(13,querys.length()-13);
	istringstream is(str);
	
	is>>table.tableName;
	string s;
	is>>s;
	if(s!="(")
	     //errror posotion
	    cout<<"error!"<<endl;
    Attribute attr;
    while(is>>s)
    {
    	if(s=="primary")
    	{
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
	        for(i=0;i<table.attrs.size();i++)
	        {
	        	if(s==table.attrs[i].attrName)
	        	{
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
		}
		else 
		{
			attr.attrName=s;
			is>>s;
			if(s=="int")
			{
				attr.type=0;
			}
			else if(s=="float")
			{
				attr.type=-1;
			}
			else if(s=="char")
			{
				is>>s;
				if(s!="(")
				     //errror posotion
	                cout<<"error!"<<endl;	
                is>>s;
                int num;
                if(InverttoInt(s,&num)==false)
                    //errror posotion
	                cout<<"error!"<<endl;
	            else
	            {
	            	attr.type=num;
				}
				
				is>>s;
				if(s!=")")
				    //errror posotion
	                cout<<"error!"<<endl;
	            
	            is>>s;
	            if(s=="unique")
	            {
	            	table.hasIndex =true;
	            	attr.isIndex=true;
	            	attr.unique=true;
	            	//设定index 
	            	is>>s;
				}
				else
				{
					attr.isIndex=false;
	            	attr.unique=false;
				}
				
				if(s==",")
				{
					table.attrs.push_back(attr);
				}
				else if(s==")")
				{
					table.attrs.push_back(attr);
				    break;
				}
				else
				{
					//errror posotion
	                cout<<"error!"<<endl;
				}
			}
		}
	}
}

void EXEC_CREATE_INDEX()
{
	if(querys[12]!=' ')
	    //errror posotion
	    cout<<"error!"<<endl;
	
	string str=querys.substr(13,querys.length()-13);
	istringstream is(str);
	string s,indexname,tablename,attrname;
	
	is>>indexname;
	is>>s;
	if(s!="on")
	{
		//errror posotion
	    cout<<"error!"<<endl;
	}
	is>>tablename;
	is>>s;
	if(s!="(")
	{
		//errror posotion
	    cout<<"error!"<<endl;
	}
	is>>attrname;
	is>>s;
	if(s!=")")
	{
		//errror posotion
	    cout<<"error!"<<endl;
	}
	is>>s;
	if(s!=";")
	{
	   	//errror posotion
	    cout<<"error!"<<endl;
	}
	//create index
}

void EXEC_DROP()
{
	if(querys[4]!=' ')
	    //errror posotion
	    cout<<"error!"<<endl;
	    
	if(querys.substr(5, 5) == "table")
	{
		if(querys[10]!=' ')
		{
			//errror posotion
	        cout<<"error!"<<endl;
		}
		string str=querys.substr(11,querys.length()-11);
	    istringstream is(str);
	    string tablename;
	    is>>tablename;
	    string s;
	    if(s!=";")
	    {
	    	//errror posotion
	        cout<<"error!"<<endl;
		}
	    //DROP TABLE BY OTHERS
	}
	else if(querys.substr(5, 5) == "index")
	{
		if(querys[10]!=' ')
		{
			//errror posotion
	        cout<<"error!"<<endl;
		}
		//drop index stunameidx;
		string str=querys.substr(11,querys.length()-11);
	    istringstream is(str);
	    string indexname;
	    is>>tablename;
	    string s;
	    if(s!=";")
	    {
	    	//errror posotion
	        cout<<"error!"<<endl;
		}
	    //DROP index BY OTHERS
	}  
	else 
	    //error position
	    cout<<"error!"<<endl;
}

void EXEC_SELECT()
{
	if(querys[6]!=' ')
	    //errror posotion
	    cout<<"error!"<<endl;
	string str=querys.substr(7,querys.length()-7);
	istringstream is(str);
}

void EXEC_INSERT()
{
	
}

void EXEC_DELETE()
{
	
}

void EXEC_PRINT()
{
	
}

void EXEC_EXIT()
{
	
}

void EXEC_FILE()
{
	
}

vector<Condition> ConditionList(TableStruct& table )
{
	
}

bool InverttoInt(string s, int& x)
{
	int i;
    x = 0;
    for(i=0;i<s.length();i++){
        if(s[i]<='9' && s[i]>='0')
            x = x * 10 + s[i] - '0';
        else 
		    return false;
    }
    return true;
}

bool InvertToFloat(string s, float& x)
{
	x=0;
	int int_f=0;
	float float_f=0;
	int i;
	int pdot;
	
	for(pdot=0;pdot<s.length();pdot++)
		if(s[pdot]=='.')
		    break;
	for(i=0;i<pdot;i++){
        if(s[i]<='9' && s[i]>='0')
            int_f = int_f * 10 + s[i] - '0';
        else 
		    return false;
    }
	x+=int_f;
	for(i=s.length()-1;i>pdot;i--){
        if(s[i]<='9' && s[i]>='0')
            float_f = (float_f+s[i] - '0')/10.0;
        else 
		    return false;
    }
    x+=float_f;
    
    return true;
}
