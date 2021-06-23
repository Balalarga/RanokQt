#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

#include "LangFunctions.h"

// Здесь описаны узлы для построения абстрактного синтаксического дерева

struct Expression
{
    friend class NodeCreator;
public:
    virtual ~Expression(){}
    virtual double GetValue() = 0;
    virtual void Reset(){};
    bool IsError()
    {
        return !error.empty();
    }
    inline void SetValue(double value)
    {
        store = value;
        ready = true;
    }
    std::string error = "";

protected:
    Expression(){}
    void operator delete(void * ptr) {
        std::free(ptr);
    }
    bool ready = false;
    double store = 0;
};


struct NumberExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        return store;
    };

protected:
    NumberExpr(double value)
    {
        ready = true;
        store = value;
    }
    using Expression::operator delete;
};


struct ConstExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(expr->GetValue());
        return store;
    };

    std::string name;
    Expression* expr;

protected:
    ConstExpr(std::string name, Expression* expr):
        name(name),
        expr(expr)
    {
        ready = false;
    }
    using Expression::operator delete;
};

struct ArgumentExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        return store;
    };

    std::string name;
    std::pair<double, double> limits;

protected:
    ArgumentExpr(std::string name, std::pair<double, double> lims):
        name(name),
        limits(lims){}
    using Expression::operator delete;
};

struct VariableExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(expr->GetValue());
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        expr->Reset();
    }

    std::string name;
    Expression* expr;

protected:
    VariableExpr(std::string name, Expression* expr):
        name(name),
        expr(expr){}
    using Expression::operator delete;
};

struct UnaryExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
        {
            if(op)
                SetValue(op(expr->GetValue()));
            else
                error = "Unknown operation: "+opName;
        }
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        expr->Reset();
    }

    std::string opName;
    UnaryOp op;
    Expression* expr;

protected:
    UnaryExpr(std::string opName, Expression* expr):
        opName(opName),
        op(LangFunctions::FindUnaryOp(opName)),
        expr(expr){}
    using Expression::operator delete;
};

struct BinaryExpr: public Expression
{

    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
        {
            if(op)
                SetValue(op(left->GetValue(), right->GetValue()));
            else
                error = "Unknown binary operation "+opName;
        }
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        left->Reset();
        right->Reset();
    }

    std::string opName;
    BinaryOp op;
    Expression* left, *right;

protected:
    BinaryExpr(std::string opName, Expression* left,
               Expression* right):
        opName(opName),
        op(LangFunctions::FindBinaryOp(opName)),
        left(left),
        right(right){}
    using Expression::operator delete;
};

struct FunctionExpr: public Expression
{
    friend class NodeCreator;
public:
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(func(arg->GetValue()));
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        arg->Reset();
    }

    FunctionRef func;
    Expression* arg;

protected:
    FunctionExpr(FunctionRef func, Expression* arg):
        func(func),
        arg(arg){}
    using Expression::operator delete;
};

#endif // NODE_H
