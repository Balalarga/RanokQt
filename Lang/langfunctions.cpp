#include "langfunctions.h"
#include <cmath>

std::map<std::string, FunctionRef> LangFunctions::functions =
{
    {"abs",   std::abs},
    {"sqrt",  std::sqrt},
    {"sin",   std::sin},
    {"cos",   std::cos},
    {"tan",   std::tan},
    {"atan",  std::atan},
    {"asin",  std::asin},
    {"acos",  std::acos},
    {"cosh",  std::cosh},
    {"sinh",  std::sinh},
    {"tanh",  std::tanh},
    {"exp",   std::exp},
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
