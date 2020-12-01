#include "token.h"
#include <cstring>
#include <istream>

Token::Token(const TokenType type, const std::string &value, const Position &start, const Position &end)
{
    this->type = type;
    this->value = value;
    this->start = start;
    this->end = end;
}
Token::Token(const TokenType type, const Position &start, const Position &end)
{
    this->type = type;
    this->start = start;
    this->end = end;
    this->value = "none";
}

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    std::string buffer = "";
    switch (token.type)
    {
    case TokenType::ARROW:
        buffer = "ARROW";
        break;
    case TokenType::AS_KW:
        buffer = "AS_KW";
        break;
    case TokenType::ASSIGN:
        buffer = "ASSIGN";
        break;
    case TokenType::BREAK_KW:
        buffer = "BREAK_KW";
        break;
    case TokenType::CHAR_LITERAL:
        buffer = "CHAR_LITERAL";
        break;
    case TokenType::COLON:
        buffer = "COLON";
        break;
    case TokenType::COMMA:
        buffer = "COMMA";
        break;
    case TokenType::COMMENT:
        buffer = "COMMENT";
        break;
    case TokenType::CONST_KW:
        buffer = "CONST_KW";
        break;
    case TokenType::CONTINUE_KW:
        buffer = "CONTINUE_KW";
        break;
    case TokenType::DIV:
        buffer = "DIV";
        break;
    case TokenType::DOUBLE_LITERAL:
        buffer = "DOUBLE_LITERAL";
        break;
    case TokenType::ELSE_KW:
        buffer = "ELSE_KW";
        break;
    case TokenType::EQ:
        buffer = "EQ";
        break;
    case TokenType::FN_KW:
        buffer = "FN_LW";
        break;
    case TokenType::GE:
        buffer = "GE";
        break;
    case TokenType::GT:
        buffer = "GT";
        break;
    case TokenType::IDENT:
        buffer = "IDENT";
        break;
    case TokenType::IF_KW:
        buffer = "IF_KW";
        break;
    case TokenType::L_BRACE:
        buffer = "L_BRACE";
        break;
    case TokenType::R_BRACE:
        buffer = "R_BRACE";
        break;
    case TokenType::L_PAREN:
        buffer = "L_PAREN";
        break;
    case TokenType::LE:
        buffer = "LE";
        break;
    case TokenType::LET_KW:
        buffer = "LET_KW";
        break;
    case TokenType::LT:
        buffer = "LT";
        break;
    case TokenType::MINUS:
        buffer = "MINUS";
        break;
    case TokenType::MUL:
        buffer = "MUL";
        break;
    case TokenType::NEQ:
        buffer = "EQ";
        break;
    case TokenType::PLUS:
        buffer = "PLUS";
        break;
    case TokenType::R_PAREN:
        buffer = "R_PAREN";
        break;
    case TokenType::RETURN_KW:
        buffer = "RETURN_KW";
        break;
    case TokenType::SEMICOLON:
        buffer = "SEMICOLON";
        break;
    case TokenType::STRING_LITERAL:
        buffer = "STRING_LITERAL";
        break;
    case TokenType::TY:
        buffer = "TY";
        break;
    case TokenType::NONE:
        buffer = "NONE";
    default:
        break;
    }
    os << buffer << " " << token.value << " " << token.start << " " << token.end;
    return os;
}