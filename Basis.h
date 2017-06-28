#ifndef MINISQL_BASIS_H
#define MINISQL_BASIS_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

const int TYPE_INT = 0;
const int TYPE_FLOAT = -1;

class Data {
public:
    int type;

public:
    virtual ~Data() {
    };

    virtual void print() const =0;
};

class DataF:public Data {
public:
    float x;

    DataF(float f) : x(f) {
        type = -1;
    }
    void print() const {
        cout << x << " " << endl;
    }
};

class DataI:public Data {
public:
    int x;

    DataI(int i) : x(i) {
        type = 0;
    }
    void print() const {
        cout << x << " " << endl;
    }
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
    void print() const {
        cout << x << " " << endl;
    }
};

//typedef vector<Data *> Tuple;

class Tuple {
public:
    vector<Data *> data;

    Tuple() {
    }

    Tuple(const Tuple &t) {
        for (auto tValue:t.data) {
            switch (tValue->type) {
                case TYPE_FLOAT:
                    this->data.push_back(new DataF(dynamic_cast<DataF *>(tValue)->x));
                    break;
                case TYPE_INT:
                    this->data.push_back(new DataI(dynamic_cast<DataI *>(tValue)->x));
                    break;
                default:
                    this->data.push_back(new DataS(dynamic_cast<DataS *>(tValue)->x));
                    break;
            }
        }
    }

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

/*class TableException: public std::exception{
public:
    TableException(string s) : text(s) {
    }

    string what() {
        return text;
    };
private:
    string text;
};*/

#endif //MINISQL_BASIS_H

