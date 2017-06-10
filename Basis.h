//
// Created by Kira on 2017/6/8.
//

#ifndef MINISQL_BASIS_H
#define MINISQL_BASIS_H

#include <string>

using namespace std;

class Data {
public:
    int type;

public:
    virtual ~Data() {
    };
};

class DataF:public Data {
public:
    float x;

    DataF(float f) : x(f) {
        type = 0;
    }

	bool operator<(const DataF& n) { return (x < n.x); };
	bool operator==(const DataF& n) { return (x == n.x); };
	bool operator<=(const DataF& n) { return (*this < n) || (*this == n); };
	bool operator>(const DataF& n) { return !(*this <= n); };
	bool operator!=(const DataF& n) { return !(*this == n); };
	bool operator>=(const DataF& n) { return !(*this < n); };
};

class DataI:public Data {
public:
    int x;

    DataI(int i) : x(i) {
        type = -1;
    }

	bool operator<(const DataI& n) { return (x < n.x); };
	bool operator==(const DataI& n) { return (x == n.x); };
	bool operator<=(const DataI& n) { return (*this < n) || (*this == n); };
	bool operator>(const DataI& n) { return !(*this <= n); };
	bool operator!=(const DataI& n) { return !(*this == n); };
	bool operator>=(const DataI& n) { return !(*this < n); };
};

class DataS:public Data {
public:
    string x;

    DataS(string c) : x(c) {
        type = c.length();
        if (type == 0) {
            type = 1;
        }
    }

	bool operator<(const DataS& n) { return (x < n.x); };
	bool operator==(const DataS& n) { return (x == n.x); };
	bool operator<=(const DataS& n) { return (*this < n) || (*this == n); };
	bool operator>(const DataS& n) { return !(*this <= n); };
	bool operator!=(const DataS& n) { return !(*this == n); };
	bool operator>=(const DataS& n) { return !(*this < n); };
};

class tuple
{
public:
	tuple() {};
	~tuple() {};
	vector<Data> data;
};

typedef enum {
    EQ, LEQ, L, GEQ, G, NEQ
} CONDITION;

class Condition {
    Data d;
    CONDITION flag;
};

class TableException: public std::exception{
public:
    TableException(string s):text(s){}
    string what(){
        return text;
    };
private:
    string text;
};

#endif //MINISQL_BASIS_H
