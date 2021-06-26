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

    ConstExpr* GetConst(const std::string& name) const;
    ArgumentExpr* GetArgument(const std::string& name) const;
    VariableExpr* GetVariable(const std::string& name) const;
    VariableExpr* GetLastVar() const;


private:
    std::unordered_map<std::string, ArgumentExpr*> m_arguments;
    std::map<std::string, VariableExpr*> m_variables;
    std::map<std::string, ConstExpr*> m_constants;
    VariableExpr* m_lastAddedVariable;
    bool HasName(const std::string& name) const;
};

#endif // SYMBOLTABLE_H
