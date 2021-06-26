#include "SymbolTable.h"

SymbolTable::SymbolTable():
    m_lastAddedVariable(nullptr)
{

}

void SymbolTable::Add(Expression *expr)
{
    if(auto it = dynamic_cast<ConstExpr*>(expr))
    {
        if(!HasName(it->name))
            m_constants[it->name] = it;
    }
    else if(auto it = dynamic_cast<VariableExpr*>(expr))
    {
        if(!HasName(it->name))
            m_variables[it->name] = it;
    }
    else if(auto it = dynamic_cast<ArgumentExpr*>(expr))
    {
        if(!HasName(it->name))
            m_arguments[it->name] = it;
    }
}

void SymbolTable::ForceAdd(Expression* expr)
{
    if(auto it = dynamic_cast<ConstExpr*>(expr))
        m_constants[it->name] = it;

    else if(auto it = dynamic_cast<VariableExpr*>(expr))
        m_variables[it->name] = it;

    else if(auto it = dynamic_cast<ArgumentExpr*>(expr))
        m_arguments[it->name] = it;
}

ArgumentExpr *SymbolTable::GetArgAt(size_t id) const
{
    if(id >= m_arguments.size())
        return nullptr;

    auto it = m_arguments.begin();
    std::advance(it, id);
    return it->second;
}

std::vector<ArgumentExpr*> SymbolTable::GetAllArgs() const
{
    std::vector<ArgumentExpr*> args;
    for(auto& i: m_arguments)
        args.push_back(i.second);
    return args;
}

Expression* SymbolTable::Get(const std::string &name) const
{
    auto constIter = m_constants.find(name);
    if(constIter != m_constants.end())
        return constIter->second;

    auto varIter = m_variables.find(name);
    if(varIter != m_variables.end())
        return varIter->second;

    auto argIter = m_arguments.find(name);
    if(argIter != m_arguments.end())
        return argIter->second;

    return nullptr;
}

void SymbolTable::Merge(const SymbolTable &oth)
{
    for(auto& i: oth.m_arguments)
        m_arguments[i.first] = i.second;

    for(auto& i: oth.m_variables)
        m_variables[i.first] = i.second;

    for(auto& i: oth.m_constants)
        m_constants[i.first] = i.second;

    m_lastAddedVariable = oth.m_lastAddedVariable;
}

void SymbolTable::Concat(const SymbolTable &oth)
{
    for(auto& i: oth.m_arguments)
    {
        if(m_arguments.find(i.first) == m_arguments.end())
            m_arguments[i.first] = i.second;
    }

    for(auto& i: oth.m_variables)
    {
        if(m_variables.find(i.first) == m_variables.end())
        {
            m_variables[i.first] = i.second;
            m_lastAddedVariable = i.second;
        }
    }

    for(auto& i: oth.m_constants)
    {
        if(m_constants.find(i.first) == m_constants.end())
            m_constants[i.first] = i.second;
    }
}

ConstExpr *SymbolTable::GetConst(const std::string &name) const
{
    auto it = m_constants.find(name);
    if(it != m_constants.end())
        return it->second;
    return nullptr;
}

ArgumentExpr *SymbolTable::GetArgument(const std::string &name) const
{
    auto it = m_arguments.find(name);
    if(it != m_arguments.end())
        return it->second;
    return nullptr;
}

VariableExpr *SymbolTable::GetVariable(const std::string &name) const
{
    auto it = m_variables.find(name);
    if(it != m_variables.end())
        return it->second;
    return nullptr;
}

VariableExpr *SymbolTable::GetLastVar() const
{
    return m_lastAddedVariable;
}

bool SymbolTable::HasName(const std::string &name) const
{
    return Get(name) != nullptr;
}
