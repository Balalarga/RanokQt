#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType
{
    Begin,
    Id,
    Number,
    Assign,
    Endline,
    End,
    ParenOpen,
    ParenClose,
    Comma,
    Pow,
    Multiply,
    Divide,
    Plus,
    Minus,
    Cross,
    Union
};

struct Token
{
    Token(TokenType type = TokenType::Begin):
        type(type){}
    Token(TokenType type, double value):
        type(type),
        value(value){}
    Token(TokenType type, std::string name):
        type(type),
        name(name){}
    Token(TokenType type, char c):
        type(type),
        name(std::string(1, c))
    {
    }
    TokenType type;
    double value = 0;
    std::string name = "";

    std::string ToString()
    {
        switch (type) {
        case TokenType::Begin:
            return "Begin";
        case TokenType::Id:
            return "Id : "+name;
        case TokenType::Number:
            return "Number : "+std::to_string(value);
        case TokenType::Endline:
            return "Endline";
        case TokenType::End:
            return "End";
        case TokenType::ParenOpen:
            return "ParenOpen";
        case TokenType::ParenClose:
            return "ParenClose";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Pow:
            return "Pow";
        case TokenType::Multiply:
            return "Multiply";
        case TokenType::Divide:
            return "Divide";
        case TokenType::Plus:
            return "Plus";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Cross:
            return "Cross";
        case TokenType::Union:
            return "Union";
        case TokenType::Assign:
            return "Assign";
        }
        return "";
    }

    bool operator !=(TokenType type)
    {
        return this->type != type;
    }
    bool operator ==(TokenType type)
    {
        return this->type == type;
    }
};

#endif // TOKEN_H
