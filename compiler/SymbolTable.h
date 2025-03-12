#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct {
    string identifier;
    int offset;
    bool use = false;
} desc_identifier;

class SymbolTable {
    public:
        SymbolTable(){};
        virtual ~SymbolTable(){};
        void print();
        int size();
        void addIdentifier(desc_identifier id);
        int getIndex(string name);
        int getOffset(string name);
        void setUse(string name);
        bool isEachIdUsed();

    private:
        vector<desc_identifier> vect;
};
