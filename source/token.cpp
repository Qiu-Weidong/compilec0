#include "token.h"
#include <cstring>
#include <istream>

Token::Token(const TokenType type, const long long value, const position &start, const position &end)
{
    this->type = type;
    // std::cout << (int)type << std::endl;
    this->start = start;
    this->end = end;
    if (type == TokenType::UINT_LITERAL) // 如果是无符号整数常量值
        this->value.n = value;
    else if (type == TokenType::DOUBLE_LITERAL) // 如果是浮点数，则将value转换为浮点数
        this->value.f = (double)value;
    else if (type == TokenType::CHAR_LITERAL) // 如果是字符字面值，则将value转换为字符
        this->value.c = (char)value;
    else
        this->value.s = nullptr;
}
Token::Token(const TokenType type, const position &start, const position &end)
{
    this->type = type;
    this->start = start;
    this->end = end;
    this->value.s = nullptr;
}
Token::Token(const TokenType type, const double value, const position &start, const position &end)
{
    this->type = type;
    this->start = start;
    this->end = end;
    if (type == TokenType::UINT_LITERAL) // 如果是无符号整数常量值
        this->value.n = (int)value;
    else if (type == TokenType::DOUBLE_LITERAL) // 如果是浮点数，则将value转换为浮点数
        this->value.f = value;
    else if (type == TokenType::CHAR_LITERAL) // 如果是字符字面值，则将value转换为字符
        this->value.c = (char)value;
    else
        this->value.s = nullptr;
}
Token::Token(const TokenType type, const char value, const position &start, const position &end)
{
    this->type = type;
    this->start = start;
    this->end = end;
    if (type == TokenType::UINT_LITERAL) // 如果是无符号整数常量值
        this->value.n = (int)value;
    else if (type == TokenType::DOUBLE_LITERAL) // 如果是浮点数，则将value转换为浮点数
        this->value.f = (double)value;
    else if (type == TokenType::CHAR_LITERAL) // 如果是字符字面值，则将value转换为字符
        this->value.c = value;
    else
        this->value.s = nullptr;
}

Token::Token(const TokenType type, const char *value, const position &start, const position &end)
{
    this->type = type;
    this->start = start;
    this->end = end;
    if (type == TokenType::STRING_LITERAL || type == TokenType::IDENT || type == TokenType::COMMENT)
    {
        int len = strlen(value);
        this->value.s = new char[len + 1];
        strcpy(this->value.s, value);
    }
    else
        this->value.s = nullptr;
}
Token::~Token()
{
    if ((type == TokenType::IDENT || type == TokenType::COMMENT||
         type == TokenType::STRING_LITERAL) &&
        value.s != nullptr)
        delete[] value.s;
}

Token::Token(const Token &token)
{
    this->type = token.type;
    this->start = token.start;
    this->end = token.end;
    if ((type == TokenType::IDENT ||type == TokenType::COMMENT||
         type == TokenType::STRING_LITERAL) &&
        token.value.s != nullptr)
    {
        int len = strlen(token.value.s);
        value.s = new char[len+1];
        strcpy(this->value.s,token.value.s);
    }
    else value.n = token.value.n;
}
Token::Token(Token && token)
{
    this->type = token.type;
    this->start = token.start;
    this->end = token.end;
    this->value.s = token.value.s;
    token.value.s = nullptr;
}
Token & Token::operator=(const Token & token)
{
    this->type = token.type;
    this->start = token.start;
    this->end = token.end;
    if ((type == TokenType::IDENT ||type == TokenType::COMMENT||
         type == TokenType::STRING_LITERAL) &&
        token.value.s != nullptr)
    {
        int len = strlen(token.value.s);
        value.s = new char[len+1];
        strcpy(this->value.s,token.value.s);
    }
    else
    {
        value.n = token.value.n;
    }
    
    return *this;
}
Token & Token::operator=(Token && token)
{
    this->type = token.type;
    this->start = token.start;
    this->end = token.end;
    this->value.s = token.value.s;
    token.value.s = nullptr;
    return *this;
}

std::ostream & operator<<(std::ostream & os,const Token & token)
{
    // os << "fuck\n";
    // os << (int)token.type;
    switch (token.type)
    {
    case TokenType::ARROW:
        os << "ARROW:-> start:" << token.start << "end:" << token.end  << std::endl;
        break;
    case TokenType::AS_KW:
        os << "AS_KW:as start"<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::ASSIGN:
        os << "ASSIGN:= start"<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::BREAK_KW:
        os << "BREAK_KW start"<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::CHAR_LITERAL:
        os << "CHAR_LITERAL:start "<< token.getCharValue()<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::COLON:
        os << "COLON:start " << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::COMMA:
        os << "COMMA:,start "<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::COMMENT:
        os << "COMMENT:"<< token.getStringValue() << "start:" << token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::CONST_KW:
        os << "CONST_KW start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::CONTINUE_KW:
        os << "CONTINUE_KW start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::DIV:
        os << "DIV:/ start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::DOUBLE_LITERAL:
        os << "DOUBLE_LITERAL:" << token.getDoubleValue()<< "start:"<< token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::ELSE_KW:
        os << "ELSE_KW start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::EQ:
        os << "EQ:== start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::FN_KW:
        os << "FN_KW start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::GE:
        os << "GE:>= start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::GT:
        os << "GT:> start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::IDENT:
        os << "IDENT:"<< token.getStringValue()<< " start:"<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::IF_KW:
        os << "IF_KW start "<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::L_BRACE:
        os << "L_BRACE:{ start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::L_PAREN:
        os << "L_PAREN:( start"<< token.start << "end:" << token.end << std::endl;
        break;
    case TokenType::LE:
        os << "LE:<= start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::LET_KW:
        os << "LET_KW start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::LT:
        os << "LT:< start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::MINUS:
        os << "MINUS:- start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::MUL:
        os << "MUL:* start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::NEQ:
        os << "NEQ:!= start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::PLUS:
        os << "PLUS:+ start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::R_BRACE:
        os << "R_BRACE:} start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::R_PAREN:
        os << "R_PAREN:) start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::RETURN_KW:
        os << "RETURN_KW:start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::SEMICOLON:
        os << "SEMICOLON:; start" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::STRING_LITERAL:
        os << "STRING_LITERAL:"<< token.getStringValue()<< "start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::UINT_LITERAL:
        os << "UINT_LITERAL:" << token.getIntegerValue() << "start:" << token.start << "end:" << token.end<< std::endl;
        break;
    case TokenType::WHILE_KW:
        os << "WHILE_KW start:" << token.start << "end:" << token.end<< std::endl;
        break;
    default:
        break;
    }
    return os;
}