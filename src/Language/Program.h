#ifndef PROGRAM_H
#define PROGRAM_H

#include "Node.h"
#include <map>
#include <set>
#include "Utils.h"

struct ArgData
{
    std::pair<double, double> limits;
    std::string name;
};

// Класс, содержащий информацию о абстрактном синтаксическом дереве
// и его аргументах, константах, и переменных

class Program
{
public:
    Program();

    double Compute(std::map<std::string, double> arguments);
    double Compute(Vector3d args);

    std::vector<ArgData> GetArgs();
    void PrintTreeDepth(int depth);
    std::string GetError();
    bool IsError();

    void AddArg(std::string& name, std::pair<double, double> limits);
    std::shared_ptr<ArgumentExpr> findArgument(const std::string& name);

    void AddVar(std::string& name, std::shared_ptr<Expression> expr);
    std::shared_ptr<VariableExpr> findVariable(const std::string& name);

    void AddConst(std::string& name, std::shared_ptr<Expression> expr);
    std::shared_ptr<ConstExpr> findConstant(const std::string& name);

    void AddResult(std::shared_ptr<Expression> expr);

    bool MergeProgram(const Program* program);

private:
    std::string error = "";

    std::shared_ptr<Expression> result;
    std::map<std::string, std::shared_ptr<ArgumentExpr>> arguments;
    std::map<std::string, std::shared_ptr<VariableExpr>> variables;
    std::map<std::string, std::shared_ptr<ConstExpr>> constants;

    void PrintNode(Expression* node, std::set<std::string>& vars, int currDepth, int maxDepth);

};

#endif // PROGRAM_H
