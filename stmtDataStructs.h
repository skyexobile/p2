#include <vector>
#include <string>
#include "Field.h"
#include <iostream>
using namespace std;

#ifndef STMTDATASTRUCTS_H
#define STMTDATASTRUCTS_H

class createTableData {
    public:
    vector<string> field_names;
    vector<enum FIELD_TYPE> field_types;
    string relationName;
    friend ostream& operator<< (ostream& out, const createTableData& crTableObj) {
        out << "crTableObj" << endl;
        out << "relation name: " << crTableObj.relationName << '\n';
        out << "field_names: " << '\n';
        for (const auto& i:crTableObj.field_names) {
            out << i << '\t';
        }
        out << '\n';
        out << "field_types: " << '\n';
        for (const auto& i:crTableObj.field_types) {
            if (i == INT) {
                out << "INT" << '\t';
            } else if (i == STR20) {
                out << "STR20" << '\t';
            }
        }
        out << '\n';
        return out;
    }
};

class insertData {
    public:
        vector<string> field_names;
        vector<string> field_values;
        string relationName;

        friend ostream& operator<< (ostream& out, const insertData& inDataObj) {
            out << "inDataObj" << endl;
            out << "field name: " << '\n';
            for (const auto& i:inDataObj.field_names) {
                out << i << '\t';
            }
            out << '\n';
            out << "field_values: " << '\n';
            for (const auto& i:inDataObj.field_values) {
                out << i << '\t';
            }

            out << '\n';
            return out;
          }
};

class selectData{
  public:
    vector<string> relation_names;
    vector<string> column_names;
    string orderByCol;
    friend ostream& operator<< (ostream& out, const selectData& selDataObj) {
        out << "selDataObj" << endl;
        out << "relation_names: " << '\n';
        for (const auto& i:selDataObj.relation_names) {
            out << i << '\t';
        }
        out << '\n';
        out << "column_names: " << '\n';
        for (const auto& i:selDataObj.column_names) {
            out << i << '\t';
        }
        out << '\n';
        out << "orderByCol: " << selDataObj.orderByCol << '\n';
        return out;
      }
};
class deleteData{
public:
  string relation_name;
};

#endif
