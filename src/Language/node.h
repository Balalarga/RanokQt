#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

#include "LangFunctions.h"

struct Expression
{
    Expression(){}
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
    bool ready = false;
    double store = 0;
};


struct NumberExpr: public Expression
{
    NumberExpr(double value)
    {
        ready = true;
        store = value;
    }
    virtual double GetValue() override
    {
        return store;
    };
};


struct ConstExpr: public Expression
{
    ConstExpr(std::string name, std::shared_ptr<Expression> expr):
        name(name),
        expr(expr)
    {
        ready = false;
    }
    virtual double GetValue() override
    {
        if(!ready)
            SetValue(expr->GetValue());
        return store;
    };

    std::string name;
    std::shared_ptr<Expression> expr;
};

struct ArgumentExpr: public Expression
{
    ArgumentExpr(std::string name, std::pair<double, double> lims):
        name(name),
        limits(lims){}
    virtual double GetValue() override
    {
        return store;
    };

    std::string name;
    std::pair<double, double> limits;
};

struct VariableExpr: public Expression
{
    VariableExpr(std::string name, std::shared_ptr<Expression> expr):
        name(name),
        expr(expr){}
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
    std::shared_ptr<Expression> expr;
};

struct UnaryExpr: public Expression
{
    UnaryExpr(std::string op, std::shared_ptr<Expression> expr):
        op(op),
        expr(expr){}
    virtual double GetValue() override
    {
        if(op == "-")
        {
            if(!ready)
            {
                SetValue(-expr->GetValue());
            }
            return store;
        }
        error = "Unknown operation "+op;
        return 0;
    };
    virtual void Reset() override
    {
        ready = false;
        expr->Reset();
    }

    std::string op;
    std::shared_ptr<Expression> expr;
};

struct BinaryExpr: public Expression
{
    BinaryExpr(std::string op, std::shared_ptr<Expression> left,
               std::shared_ptr<Expression> right):
        op(op),
        left(left),
        right(right){}
    virtual double GetValue() override
    {
        if(!ready)
        {
            double lVal = left->GetValue();
            double rVal = right->GetValue();
            if(op == "+")
            {
                SetValue(lVal+rVal);
            }
            else if(op == "-")
            {
                SetValue(lVal-rVal);
            }
            else if(op == "*")
            {
                SetValue(lVal*rVal);
            }
            else if(op == "/")
            {
                if(rVal == 0)
                {
                    error = "Zero division";
                    return 0;
                }
                SetValue(lVal/rVal);
            }
            else if(op == "^")
            {
                SetValue(pow(lVal, rVal));
            }
            else if(op == "&")
            {
                SetValue(lVal + rVal - sqrt(pow(lVal, 2) + pow(rVal, 2)));
            }
            else if(op == "|")
            {
                SetValue(lVal + rVal + sqrt(pow(lVal, 2) + pow(rVal, 2)));
            }
            else
                error = "Unknown operation "+op;
        }
        return store;
    };
    virtual void Reset() override
    {
        ready = false;
        left->Reset();
        right->Reset();
    }

    std::string op;
    std::shared_ptr<Expression> left, right;
};

struct FunctionExpr: public Expression
{
    FunctionExpr(FunctionRef func, std::shared_ptr<Expression> arg):
        func(func),
        arg(arg){}
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
    std::shared_ptr<Expression> arg;
};

#endif // NODE_H
