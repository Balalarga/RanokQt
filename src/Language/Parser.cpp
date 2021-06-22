#include "Parser.h"

#include "LangFunctions.h"
#include "Token.h"

#include <sstream>
#include <fstream>


using namespace std;


Parser::Parser()
{

}


void Parser::SetFile(const std::string &sourceFile)
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
    SetText(converter.str());
}

void Parser::SetText(const std::string &source)
{
    lexer.SetText(source);
}

Program *Parser::GetProgram()
{
    Program* program = new Program;
    ToNextToken();
    if(!lexer.IsError() && !IsError())
    {
        while(token != Token::Type::End)
        {
            if(token == Token::Type::Id)
            {
                if(token.name == "variable" ||
                        token.name == "var")
                    HandleVariable(*program);
                else if(token.name == "argument" ||
                        token.name == "arg")
                    HandleArgument(*program);
                else if(token.name == "constant" ||
                        token.name == "const")
                    HandleConstant(*program);
                else if(token.name == "return")
                    HandleReturn(*program);
                else
                {
                    error = "Unknown keyword " + token.name;
                }
            }
            else
            {
                error = "Unknown token " + token.name;
            }
        }
    }
    else
    {
        if(lexer.IsError())
            cout<<"Lexer error: "<<lexer.GetError()<<endl;
        else
            cout<<"Parser error: "<<error<<endl;
        delete program;
        program = nullptr;
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
        while(token != Token::Type::Endline)
        {
            ToNextToken();
            CheckToken(Token::Type::Id);
            string name = token.name;
            ToNextToken();
            pair<double, double> limit{-5, 5};
            if(token == Token::Type::ParenOpen)
            {
                ToNextToken();
                bool negative = false;
                if(token == Token::Type::Minus)
                {
                    negative = true;
                    ToNextToken();
                }
                CheckToken(Token::Type::Number);
                limit.first = token.value;
                ToNextToken();
                if(token == Token::Type::Comma)
                {
                    limit.first = negative ? -limit.first : limit.first;
                    ToNextToken();
                    negative = false;
                    if(token == Token::Type::Minus)
                    {
                        negative = true;
                        ToNextToken();
                    }
                    CheckToken(Token::Type::Number);
                    limit.second = negative ? -token.value : token.value;
                    ToNextToken();
                }
                else
                {
                    limit.second = limit.first;
                    limit.first = -limit.second;
                }
                CheckToken(Token::Type::ParenClose);
                ToNextToken();
            }
            program.AddArg(name, limit);
        }
        CheckToken(Token::Type::Endline);
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
        while(token != Token::Type::Endline)
        {
            ToNextToken();
            CheckToken(Token::Type::Id);
            string name = token.name;
            ToNextToken();
            CheckToken(Token::Type::Assign);
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
        CheckToken(Token::Type::Id);
        string name = token.name;
        ToNextToken();
        CheckToken(Token::Type::Assign);
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

void Parser::CheckToken(Token::Type expect)
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
    while(token == Token::Type::Pow      ||
          token == Token::Type::Multiply ||
          token == Token::Type::Divide)
    {
        auto prev = token;
        ToNextToken();
        node = shared_ptr<BinaryExpr>(new BinaryExpr(prev.name, node, Factor(program)));
    }
    return node;
}

std::shared_ptr<Expression> Parser::Factor(Program& program)
{
    if(token == Token::Type::Number)
    {
        auto expr = shared_ptr<NumberExpr>(new NumberExpr(token.value));
        ToNextToken();
        return expr;
    }
    else if(token == Token::Type::ParenOpen)
    {
        ToNextToken();
        auto expr = Expr(program);
        ToNextToken();
        return expr;
    }
    else if(token.type == Token::Type::Minus)
    {
        ToNextToken();
        return shared_ptr<UnaryExpr>(new UnaryExpr("-", Factor(program)));
    }
    else if(token.type == Token::Type::Id)
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
        auto func = LangFunctions::FindFunction(prev.name);
        if(func)
            return shared_ptr<FunctionExpr>(new FunctionExpr(func, Term(program)));
    }
    return nullptr;
}

std::shared_ptr<Expression> Parser::Expr(Program& program)
{
    auto node = Term(program);
    while(token == Token::Type::Minus||
          token == Token::Type::Plus ||
          token == Token::Type::Cross ||
          token == Token::Type::Union)
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
