#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Program.h"
#include "Token.h"

// Класс, который создает абстрактное синтаксическое дерево из токенов Лексера
class Parser
{
public:
    Parser();

    void SetFile(const std::string& sourceFile);
    void SetText(const std::string& source);

    Program* GetProgram();

    bool IsError();
    std::string GetError();

private:
    Lexer lexer;
    Token token;

    std::string error = "";

    // Функции обработки главных элементов языка
    void HandleArgument(Program& program);
    void HandleConstant(Program& program);
    void HandleVariable(Program& program);
    void HandleReturn(Program& program);

    void CheckToken(Token::Type expect);

    // рекурсивные функции построения дерева
    std::shared_ptr<Expression> Term(Program& program);
    std::shared_ptr<Expression> Factor(Program& program);
    std::shared_ptr<Expression> Expr(Program& program);

    void ToNextToken();
};

#endif // PARSER_H
