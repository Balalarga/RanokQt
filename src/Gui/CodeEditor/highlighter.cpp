#include "highlighter.h"
#include "Language/LangFunctions.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor::fromRgb(QRgb(0x458588)));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns = {
        "const", "CONST", "CONSTANT", "constant",
        "ARGUMENT", "arg", "ARG", "argument",
        "var", "VAR", "VARIABLE", "variable",
        "return", "RETURN"
    };

    varFormat.setFontItalic(true);
    varFormat.setForeground(QColor::fromRgb(QRgb(0xfe8019)));
    rule.pattern = QRegularExpression(QStringLiteral("[a-zA-Z_$][a-zA-Z_$0-9]*"));
    rule.format = varFormat;
    highlightingRules.append(rule);


    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression("\\b"+pattern+"\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(QColor::fromRgb(QRgb(0x504945)));
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);


    quotationFormat.setForeground(QColor::fromRgb(QRgb(0x98971a)));
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor::fromRgb(QRgb(0x8ec07c)));
    for (auto& word: LangFunctions::functions) {
        rule.pattern = QRegularExpression("\\b("+QString::fromStdString(word.first).toUpper()+"|"
                                                +QString::fromStdString(word.first).toLower()+")\\b");
        rule.format = functionFormat;
        highlightingRules.append(rule);
    }

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + match.capturedLength();
        }
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
