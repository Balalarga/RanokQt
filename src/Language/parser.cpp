#include "Parser.h"
#include "langfunctions.h"
#include <sstream>
#include <fstream>

using namespace std;

Parser::Parser(const std::string &sourceFile)
{
    fstream file(sourceFile);
    if(!file)
    {
        cout<<"Parser: couldn't open file"<<endl;
        return;
    }
    stringstream converter;
    converter<<file.rdbuf();

    file.close();

    lexer.SetText(converter.str());
}

Program Parser::GetProgram()
{
    Program program;
    ToNextToken();
    if(!lexer.IsError() && !IsError())
    {
        while(token != TokenType::End)
        {
            if(token == TokenType::Id)
            {
                if(token.name == "variable" ||
                        token.name == "var")
                    HandleVariable(program);
                else if(token.name == "argument" ||
                        token.name == "arg")
                    HandleArgument(program);
                else if(token.name == "constant" ||
                        token.name == "const")
                    HandleConstant(program);
                else if(token.name == "return")
                    HandleReturn(program);
                else
                    error = "Unknown keyword " + token.name;
            }
            else
            {
                error = "Unknown token " + token.name;
                break;
            }
        }
    }
    else
    {
        if(lexer.IsError())
            cout<<"Lexer error: "<<lexer.GetError()<<endl;
        else
            cout<<"Parser error: "<<error<<endl;
    }

    return program;
}

bool Parser::IsError()
{
    return !error.empty();
}

string Parser::GetError()
{
    return error;
}

void Parser::HandleArgument(Program &program)
{
    if(!IsError())
    {
        while(token != TokenType::Endline)
        {
            ToNextToken();
            CheckToken(TokenType::Id);
            string name = token.name;
            ToNextToken();
            pair<double, double> limit{-5, 5};
            if(token == TokenType::ParenOpen)
            {
                ToNextToken();
                bool negative = false;
                if(token == TokenType::Minus)
                {
                    negative = true;
                    ToNextToken();
                }
                CheckToken(TokenType::Number);
                limit.first = token.value;
                ToNextToken();
                if(token == TokenType::Comma)
                {
                    limit.first = negative ? -limit.first : limit.first;
                    ToNextToken();
                    negative = false;
                    if(token == TokenType::Minus)
                    {
                        negative = true;
                        ToNextToken();
                    }
                    CheckToken(TokenType::Number);
                    limit.second = negative ? -token.value : token.value;
                    ToNextToken();
                }
                else
                {
                    limit.second = limit.first;
                    limit.first = -limit.second;
                }
                CheckToken(TokenType::ParenClose);
                ToNextToken();
            }
            program.AddArg(name, limit);
        }
        CheckToken(TokenType::Endline);
        ToNextToken();
    }
    else
    {
        cout<<"Parser error: "<<error;
    }
}

void Parser::HandleConstant(Program &program)
{
    if(!IsError())
    {
        while(token != TokenType::Endline)
        {
            ToNextToken();
            CheckToken(TokenType::Id);
            string name = token.name;
            ToNextToken();
            CheckToken(TokenType::Assign);
            ToNextToken();
            auto expr = Expr(program);
            program.AddConst(name, expr);
        }
        ToNextToken();
    }
    else
    {
        cout<<"Parser error: "<<error;
    }
}

void Parser::HandleVariable(Program &program)
{
    if(!IsError())
    {
        ToNextToken();
        CheckToken(TokenType::Id);
        string name = token.name;
        ToNextToken();
        CheckToken(TokenType::Assign);
        ToNextToken();
        auto expr = Expr(program);
        program.AddVar(name, expr);
        ToNextToken();
    }
    else
    {
        cout<<"Parser error: "<<error;
    }
}

void Parser::HandleReturn(Program &program)
{
    if(!IsError())
    {
        ToNextToken();
        program.AddResult(Expr(program));
        ToNextToken();
    }
    else
        cout<<"Parser error: "<<error;
}

void Parser::CheckToken(TokenType expect)
{
    if(token != expect)
    {
        cout<<"Parser error: \n"<<"Unexpected token "+token.ToString()+
              "\n"+"Expected " + Token(expect).ToString()<<endl;
    }
}

std::shared_ptr<Expression> Parser::Term(Program& program)
{
    auto node = Factor(program);
    while(token == TokenType::Pow      ||
          token == TokenType::Multiply ||
          token == TokenType::Divide)
    {
        auto prev = token;
        ToNextToken();
        node = shared_ptr<BinaryExpr>(new BinaryExpr(prev.name, node, Factor(program)));
    }
    return node;
}

std::shared_ptr<Expression> Parser::Factor(Program& program)
{
    if(token == TokenType::Number)
    {
        auto expr = shared_ptr<NumberExpr>(new NumberExpr(token.value));
        ToNextToken();
        return expr;
    }
    else if(token == TokenType::ParenOpen)
    {
        ToNextToken();
        auto expr = Expr(program);
        ToNextToken();
        return expr;
    }
    else if(token.type == TokenType::Minus)
    {
        ToNextToken();
        return shared_ptr<UnaryExpr>(new UnaryExpr("-", Factor(program)));
    }
    else if(token.type == TokenType::Id)
    {
        auto prev = token;
        ToNextToken();
        shared_ptr<Expression> expr = program.findArgument(prev.name);
        if(expr)
            return expr;
        expr = program.findConstant(prev.name);
        if(expr)
            return expr;
        expr = program.findVariable(prev.name);
        if(expr)
            return expr;
        auto func = LangFunctions::Find(prev.name);
        if(func)
            return shared_ptr<FunctionExpr>(new FunctionExpr(func, Term(program)));
    }
    return nullptr;
}

std::shared_ptr<Expression> Parser::Expr(Program& program)
{
    auto node = Term(program);
    while(token == TokenType::Minus||
          token == TokenType::Plus ||
          token == TokenType::Cross ||
          token == TokenType::Union)
    {
        auto prev = token;
        ToNextToken();
        node = shared_ptr<BinaryExpr>(new BinaryExpr(prev.name, node, Term(program)));
    }
    return node;
}

void Parser::ToNextToken()
{
    token = lexer.NextToken();
    if(lexer.IsError())
        error = "Next token error";
}
