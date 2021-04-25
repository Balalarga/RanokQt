#ifndef PROGRAM_H
#define PROGRAM_H

#include "node.h"
#include <set>
#include <map>

struct ArgData
{
    std::pair<double, double> limits;
    std::string name;
};

class Program
{
    friend class Parser;
public:
    Program();

    double Compute(std::map<std::string, double> arguments);
    double Compute(std::vector<double> arguments);
    std::vector<ArgData> GetArgs();

    std::string GetError();
    bool IsError();

private:
    std::string error = "";

    std::shared_ptr<Expression> result;
    std::map<std::string, std::shared_ptr<ArgumentExpr>> arguments;
    std::map<std::string, std::shared_ptr<VariableExpr>> variables;
    std::map<std::string, std::shared_ptr<ConstExpr>> constants;

    std::shared_ptr<ArgumentExpr> findArgument(const std::string& name);
    std::shared_ptr<VariableExpr> findVariable(const std::string& name);
    std::shared_ptr<ConstExpr> findConstant(const std::string& name);

    void AddArg(std::string& name, std::pair<double, double> limits);
    void AddVar(std::string& name, std::shared_ptr<Expression> expr);
    void AddConst(std::string& name, double value);
};

#endif // PROGRAM_H