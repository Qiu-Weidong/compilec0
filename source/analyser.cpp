#include "analyser.h"

Analyser::Analyser(const std::vector<Token> &_tokens) : tokens(_tokens)
{
    index = 0;
    sentinel = _tokens.at(_tokens.size() - 1);
}
const Token &Analyser::peek() const
{
    return tokens[index + 1];
}

const Token &Analyser::next()
{
    index++;
    return tokens[index];
}
const Token &Analyser::current() const
{
    return tokens[index];
}
const Token &Analyser::previous()
{
    if (index <= 0)
        throw Error(ErrorCode::NoToken, "there is no previous token!");
    index--;
    return tokens[index];
}
const Token &Analyser::expect(TokenType type)
{
    if (!has_next())
        throw Error(ErrorCode::NoToken, "there is no token anymore!", sentinel.getStart());
    index++;
    if (tokens[index].getTokenType() != type)
        throw Error(ErrorCode::ExpectFail, "expect fail", current().getStart());
    return tokens[index];
}

bool Analyser::has_next() const
{
    if (index + 1 >= tokens.size())
        return false;
    return tokens[index + 1].getTokenType() != TokenType::NONE;
}
bool Analyser::has_previous() const { return index > 0; }

void Analyser::program(Program & pg)
{
    Function & _start = pg.get_start();
    VaribleTable & globals = pg.getGlobals();
    FunctionTable & ft = pg.getFunctions();
    while(has_next())
    {
        auto t = peek().getTokenType();
        if( t == TokenType::FN_KW)
            func(globals,ft);
        else if(t == TokenType::LET_KW || t == TokenType::CONST_KW)
            decl_stmt(globals,ft,_start);
        else throw Error(ErrorCode::InvalidItem,"invalid item",current().getStart());
    }
    Function & main = pg.getFunction("main",current().getStart());
    _start.addInstruction(Instruction(Operation::CALL,main.getFid()));
    std::cout << "Accepted!" << std::endl;
}

bool Analyser::isExpressionTermination(TokenType type)
{
    return type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::MUL ||
           type == TokenType::COMMA || type == TokenType::DIV || type == TokenType::EQ || 
           type == TokenType::NEQ || type == TokenType::GT || type == TokenType::GE || 
           type == TokenType::LT || type == TokenType::LE || type == TokenType::ASSIGN || 
           type == TokenType::IDENT || type == TokenType::UINT_LITERAL || 
           type == TokenType::CHAR_LITERAL || type == TokenType::STRING_LITERAL || 
           type == TokenType::DOUBLE_LITERAL || type == TokenType::L_PAREN || 
           type == TokenType::R_PAREN || type == TokenType::AS_KW || type == TokenType::TY;
}