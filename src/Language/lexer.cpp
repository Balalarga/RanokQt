#include "Lexer.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

void Lexer::SetText(const std::string& text)
{
    if(text.empty())
    {
        error = "Empty source code";
        return;
    }
    std::string data = text;
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    unsigned pivot = 0;
    Token token;
    while(token != TokenType::End)
    {
        token = ParseNextToken(data, pivot);
        m_tokens.push(token);
    }
    m_tokens.push(token);
}

Token Lexer::NextToken()
{
    if(!m_tokens.empty())
    {
        auto first = m_tokens.front();
        m_tokens.pop();
        return first;
    }
    return Token(TokenType::End);
}

string Lexer::GetError()
{
    return error;
}

bool Lexer::IsError()
{
    return !error.empty();
}

Token Lexer::ParseNextToken(const std::string& data, unsigned& pivot)
{
    while (pivot < data.size() && isspace(data[pivot]))
        pivot++;
    if(data[pivot] == '/' && data[pivot+1] == '/')
    {
        while(pivot < data.size() && data[pivot] != '\n')
            pivot++;
        pivot++;
        return ParseNextToken(data, pivot);
    }

    if(pivot >= data.size() || data[pivot] == '\0')
        return Token(TokenType::End, "\\0");
    if(data[pivot] == ';')
    {
        pivot++;
        return Token(TokenType::Endline, ';');
    }
    unsigned begin = pivot;
    if ((data[pivot+1] == '.')
            || isdigit (data[pivot])
            // allow decimal numbers without a leading 0. e.g. ".5"
            || (data[pivot] == '.' && isdigit (data[pivot+1])))
    {
        // skip sign for now
        if ((data[pivot] == '+' || data[pivot] == '-'))
            pivot++;
        while (isdigit (data[pivot]) || data[pivot] == '.')
            pivot++;
        // allow for 1.53158e+15
        if (data[pivot] == 'e' || data[pivot] == 'E')
        {
            pivot++; // skip 'e'
            if ((data[pivot]  == '+' || data[pivot]  == '-'))
                pivot++; // skip sign after e
            while (isdigit (data[pivot]))  // now digits after e
                pivot++;
        }
        string number = data.substr(begin, pivot - begin);
        double val;
        istringstream is (number);
        is >> val;

        if (is.fail () && !is.eof ())
        {
            error = "Unexpected eof";
            return Token(TokenType::End);
        }
        return Token(TokenType::Number, val);
    }

    char symbol = data[pivot++];
    switch (symbol)
    {
    case '=':
        return Token(TokenType::Assign, symbol);
    case '(':
        return Token(TokenType::ParenOpen, symbol);
    case ')':
        return Token(TokenType::ParenClose, symbol);
    case '+':
        return Token(TokenType::Plus, symbol);
    case '-':
        return Token(TokenType::Minus, symbol);
    case '*':
        return Token(TokenType::Multiply, symbol);
    case '/':
    case '\\':
        return Token(TokenType::Divide, symbol);
    case '&':
        return Token(TokenType::Union, symbol);
    case '|':
        return Token(TokenType::Cross, symbol);
    case ',':
        return Token(TokenType::Comma, symbol);
    case '^':
        return Token(TokenType::Pow, symbol);
    }
    pivot--;

    if (!isalpha (data[pivot]))
    {
        error = "Unexpected symbol "+string(1, data[pivot]);
        return Token(TokenType::End);
    }

    // we have a word (starting with A-Z) - pull it out
    while (isalnum (data[pivot]) || data[pivot] == '_')
        pivot++;
    auto word = data.substr(begin, pivot-begin);
    return Token(TokenType::Id, word);
}
