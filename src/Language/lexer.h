#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <queue>
#include "Token.h"

class Lexer
{
public:
    Lexer();
    ~Lexer();
    void SetText(const std::string& text);
    Token NextToken();
    std::string GetError();
    bool IsError();

protected:
    void FillQueue();
    Token ParseNextToken(const std::string &data, unsigned &pivot);

private:
    std::queue<Token> m_tokens;
    std::string error = "";
};

#endif // LEXER_H
