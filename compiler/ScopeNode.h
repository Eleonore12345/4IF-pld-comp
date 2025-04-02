#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct {
    string name;
    int offset;
    bool use = false;
    bool init = false;
    bool isTemp = false;
} variable;

class FunctionScopeNode;

class ScopeNode {
    public:
        ScopeNode(ScopeNode* parent = nullptr);
        virtual ~ScopeNode(){};
        virtual void print();
        variable* addVariable(string name, bool use = false, bool init = false, bool isTemp = false);
        void addChildren(ScopeNode* children);
        variable* getVariable(string name);
        ScopeNode* getParent();
        vector<ScopeNode*> getChildren();
        void setVisited();
        void checkIfEachIdUsed();
        void checkIfEachIdInit();
        variable* getNextNotUsedTempVar();
        void reset();
        bool isVisited();
        int getNbTmpVariable();
        

    protected:
        FunctionScopeNode* getFunctionParent();

        vector<variable> variable_vect;
        ScopeNode* parent;
        vector<ScopeNode*> children;
        bool visited;
};

class FunctionScopeNode : public ScopeNode {
    public:
        FunctionScopeNode(ScopeNode* parent, std::string name) : ScopeNode(parent), name(name), size(0){};
        virtual ~FunctionScopeNode(){};
        void print() override;
        int getSize();
        string getName() const;
        void incrementSize(int value);

    private:
        string name;
        int size;
};


