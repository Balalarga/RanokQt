#include "program.h"
#include <list>

using namespace std;

Program::Program()
{
    map<string, double> stdConstant{
        {"pi", 3.14159265358979323846},
        {"e", 2.71828182845904523536},
    };
    for(const auto& i: stdConstant)
        constants[i.first] = shared_ptr<ConstExpr>(new ConstExpr(i.first, i.second));
}

double Program::Compute(std::map<std::string, double> args)
{
    for(auto& i: args)
    {
        auto node = findArgument(i.first);
        if(!node)
        {
            error = "Unknown argument "+i.first;
            return 0;
        }
        node->SetValue(i.second);
    }

    double res = result->GetValue();
    result->Reset();

#ifdef DEBUG
    cout<<"\n\n";
#endif

    return res;
}

double Program::Compute(std::vector<double> args)
{
    if(args.size() != arguments.size())
    {
        error = "Invalid count of arguments";
        return 0;
    }

    int count = 0;
    for(auto& i: this->arguments)
    {
        i.second->SetValue(args[count++]);
    }

    double res = result->GetValue();

    result->Reset();

#ifdef DEBUG
    cout<<"\n\n";
#endif
    return res;
}

std::vector<ArgData> Program::GetArgs()
{
    vector<ArgData> data;
    for(auto i = arguments.begin(); i != arguments.end(); i++)
    {
        ArgData aData;
        aData.limits = i->second->limits;
        aData.name = i->first;
        data.push_back(aData);
    }
    return data;
}

string Program::GetError()
{
    return error;
}

bool Program::IsError()
{
    return !error.empty();
}

shared_ptr<ArgumentExpr> Program::findArgument(const std::string& name)
{
    auto it = arguments.find(name);
    if(it == arguments.end())
        return nullptr;
    return it->second;
}

shared_ptr<VariableExpr> Program::findVariable(const std::string& name)
{
    auto it = variables.find(name);
    if(it == variables.end())
        return nullptr;
    return it->second;
}

shared_ptr<ConstExpr> Program::findConstant(const std::string& name)
{
    auto it = constants.find(name);
    if(it == constants.end())
        return nullptr;
    return it->second;
}

void Program::AddArg(string& name, std::pair<double, double> limits)
{
    if(!findArgument(name))
        arguments[name] = shared_ptr<ArgumentExpr>(new ArgumentExpr(name, limits));
    else
        error = "Argument "+name+" alredy exists";
}

void Program::AddVar(string& name, std::shared_ptr<Expression> expr)
{
    if(!findVariable(name))
        variables[name] = shared_ptr<VariableExpr>(new VariableExpr(name, expr));
    else
        error = "Variable "+name+" alredy exists";
}

void Program::AddConst(string& name, double value)
{
    if(!findArgument(name))
        constants[name] = shared_ptr<ConstExpr>(new ConstExpr(name, value));
    else
        error = "Constant "+name+" alredy exists";
}
