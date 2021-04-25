#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

class Lexer
{
public:
    Lexer();
    void SetText(const std::string& text);
    Token NextToken();
    std::string GetError();
    bool IsError();

private:
    std::string error = "";
    std::string data;
    unsigned dataSize = 0;
    unsigned pivot = 0;
};

#endif // LEXER_H
