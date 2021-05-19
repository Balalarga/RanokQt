#ifndef PROGRAM_H
#define PROGRAM_H

#include "node.h"
#include <glm/glm.hpp>
#include <map>

struct ArgData
{
    std::pair<double, double> limits;
    std::string name;
};

class Program
{
public:
    Program();

    double Compute(std::map<std::string, double> arguments);
    double Compute(glm::vec3 args);

    std::vector<ArgData> GetArgs();

    std::string GetError();
    bool IsError();

    void AddArg(std::string& name, std::pair<double, double> limits);
    std::shared_ptr<ArgumentExpr> findArgument(const std::string& name);

    void AddVar(std::string& name, std::shared_ptr<Expression> expr);
    std::shared_ptr<VariableExpr> findVariable(const std::string& name);

    void AddConst(std::string& name, std::shared_ptr<Expression> expr);
    std::shared_ptr<ConstExpr> findConstant(const std::string& name);

    void AddResult(std::shared_ptr<Expression> expr);

private:
    std::string error = "";

    std::shared_ptr<Expression> result;
    std::map<std::string, std::shared_ptr<ArgumentExpr>> arguments;
    std::map<std::string, std::shared_ptr<VariableExpr>> variables;
    std::map<std::string, std::shared_ptr<ConstExpr>> constants;


};

#endif // PROGRAM_H
