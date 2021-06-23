#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Node.h"
#include <vector>
#include <unordered_map>

class SymbolTable
{
public:
    SymbolTable();

    void Add(Expression *expr);
    void ForceAdd(Expression* expr);

    ArgumentExpr* GetArgAt(size_t id) const;
    std::vector<ArgumentExpr *> GetAllArgs() const;

    Expression *Get(const std::string& name) const;

    void Merge(const SymbolTable& oth);
    void Concat(const SymbolTable& oth);

    ConstExpr* GetConst(const std::string& name) const
    {
        auto it = m_constants.find(name);
        if(it != m_constants.end())
            return it->second;
        return nullptr;
    }
    ArgumentExpr* GetArgument(const std::string& name) const
    {
        auto it = m_arguments.find(name);
        if(it != m_arguments.end())
            return it->second;
        return nullptr;
    }
    VariableExpr* GetVariable(const std::string& name) const
    {
        auto it = m_variables.find(name);
        if(it != m_variables.end())
            return it->second;
        return nullptr;
    }

private:
    std::unordered_map<std::string, ArgumentExpr*> m_arguments;
    std::map<std::string, VariableExpr*> m_variables;
    std::map<std::string, ConstExpr*> m_constants;

    bool HasName(const std::string& name) const;
};

#endif // SYMBOLTABLE_H
