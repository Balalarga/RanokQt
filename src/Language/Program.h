#ifndef PROGRAM_H
#define PROGRAM_H

#include <map>
#include <set>
#include "Utils.h"
#include "SymbolTable.h"
#include "NodeCreator.h"


// Класс, содержащий информацию о абстрактном синтаксическом дереве
// и его аргументах, константах, и переменных

class Program
{
public:
    Program();

    double Compute(std::map<std::string, double> arguments);
    double Compute(Vector3d args);

    std::string GetError();
    bool IsError();

    void SetResult(Expression* expr);

    bool MergeProgram(const Program* program);

    void PrintTreeDepth(int depth);
    SymbolTable& GetSymbolTable();

private:
    Expression* resultNode;
    SymbolTable m_symbolTable;

    std::string error = "";

    void PrintNode(Expression* node, std::set<std::string>& vars, int currDepth, int maxDepth);
};

#endif // PROGRAM_H
