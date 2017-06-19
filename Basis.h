//
// Created by Kira on 2017/6/8.
//

#ifndef MINISQL_BASIS_H
#define MINISQL_BASIS_H

#include <string>
#include <vector>

using namespace std;

const int TYPE_INT = 0;
const int TYPE_FLOAT = -1;

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
        type = -1;
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
        type = 0;
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

//typedef vector<Data *> Tuple;

class Tuple {
public:
    vector<Data *> data;
    ~Tuple() {
        for (int i = 0; i < data.size(); i++)
            delete data[i];
    }
};

typedef enum {
    EQ, LEQ, L, GEQ, G, NEQ
} CONDITION;

class Condition {
public:
    size_t attrIndex;
    Data *d;
    CONDITION flag;
    ~Condition() {
        delete d;
    }
};

class TableException: public std::exception{
public:
    TableException(string s) : text(s) {
    }

    string what() {
        return text;
    };
private:
    string text;
};

#endif //MINISQL_BASIS_H
