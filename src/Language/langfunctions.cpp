#include "LangFunctions.h"
#include <cmath>

std::map<std::string, FunctionRef> LangFunctions::functions =
{
    {"abs",   std::abs},
    {"sqrt",  std::sqrt},
    {"sin",   std::sin},
    {"cos",   std::cos},
    {"tan",   std::tan},
    {"arctan",  std::atan},
    {"arcsin",  std::asin},
    {"arccos",  std::acos},
    {"cosh",  std::cosh},
    {"sinh",  std::sinh},
    {"tanh",  std::tanh},
    {"exp",   std::exp},
    {"ln",    std::log},
    {"log",   std::log},
    {"log10", std::log10},
    {"log2",  std::log2},
    {"ceil",  std::ceil},
    {"floor", std::floor},
};

FunctionRef LangFunctions::Find(std::string name)
{
    auto it = functions.find(name);
    return it == functions.end() ? nullptr: it->second;
}

std::string LangFunctions::Find(FunctionRef func)
{
    for(auto& i: functions)
        if(i.second == func)
            return i.first;
    return "unknown";
}
