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
    TokenType type;
    double value = 0;
    std::string name = "";

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
        name(std::string(1, c)){}

    std::string ToString()
    {
        switch (type) {
        case TokenType::Begin:
            return "TokenType::Begin";
        case TokenType::Id:
            return "TokenType::Id : "+name;
        case TokenType::Number:
            return "TokenType::Number : "+std::to_string(value);
        case TokenType::Endline:
            return "TokenType::Endline";
        case TokenType::End:
            return "TokenType::End";
        case TokenType::ParenOpen:
            return "TokenType::ParenOpen";
        case TokenType::ParenClose:
            return "TokenType::ParenClose";
        case TokenType::Comma:
            return "TokenType::Comma";
        case TokenType::Pow:
            return "TokenType::Pow";
        case TokenType::Multiply:
            return "TokenType::Multiply";
        case TokenType::Divide:
            return "TokenType::Divide";
        case TokenType::Plus:
            return "TokenType::Plus";
        case TokenType::Minus:
            return "TokenType::Minus";
        case TokenType::Cross:
            return "TokenType::Cross";
        case TokenType::Union:
            return "TokenType::Union";
        case TokenType::Assign:
            return "TokenType::Assign";
        }
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
