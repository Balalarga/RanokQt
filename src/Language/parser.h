#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Program.h"

// Класс, который создает абстрактное синтаксическое дерево из токенов Лексера
class Parser
{
public:
    Parser(const std::string& sourceFile);

    Program GetProgram();

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

    void CheckToken(TokenType expect);

    // рекурсивные функции построения дерева
    std::shared_ptr<Expression> Term(Program& program);
    std::shared_ptr<Expression> Factor(Program& program);
    std::shared_ptr<Expression> Expr(Program& program);

    void ToNextToken();
};

#endif // PARSER_H
