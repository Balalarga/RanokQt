#ifndef LANGFUNCTIONS_H
#define LANGFUNCTIONS_H

#include <map>
#include <functional>

typedef double(*FunctionRef)(double);

class LangFunctions
{
public:
    LangFunctions() = delete;
    static std::map<std::string, FunctionRef> functions;
    static FunctionRef Find(std::string);
    static std::string Find(FunctionRef);
};

#endif // LANGFUNCTIONS_H
