#include "analyser.h"
#include "token.h"
#include "error.h"

//////////////
/// \brief program -> item*
//////////////
void Analyser::program()
{
    while(has_next())
    {
        item();
    }
}

//////////////////
/// \brief item -> function | decl_stmt
//////////////////
void Analyser::item()
{
    auto t = peek().getTokenType();
    if(t == TokenType::FN_KW)
    {
        func();
    }
    else if(t == TokenType::LET_KW || t == TokenType::CONST_KW)
    {
        decl_stmt();
    }
    else throw std::exception();
}

/////////////////
/// \brief decl_stmt -> let_decl_stmt | const_decl_stmt
/////////////////
void Analyser::decl_stmt()
{
    if(peek().getTokenType() == TokenType::LET_KW)
    {
        let_decl_stmt();
    }
    else if(peek().getTokenType() == TokenType::CONST_KW)
    {
        const_decl_stmt();
    }
    else throw std::exception();
}

//////////////////
/// \brief function -> 'fn' IDENT '(' function_param_list? ')' '->' ty block_stmt
//////////////////
void Analyser::func()
{
    expect(TokenType::FN_KW);
    expect(TokenType::IDENT);
    expect(TokenType::L_PAREN);
    func_param_list();
    expect(TokenType::R_PAREN);
    expect(TokenType::ARROW);
    expect(TokenType::TY);
    block_stmt();
}

/////////////////////
/// \brief function_param -> 'const'? IDENT ':' ty
/////////////////////
void Analyser::func_param()
{
    if(!has_next()) throw std::exception();
    if(peek().getTokenType() == TokenType::CONST_KW) next();
    expect(TokenType::IDENT);
    expect(TokenType::COLON);
    expect(TokenType::TY);
}

///////////////////////
/// \brief function_param_list -> function_param (',' function_param)*
///////////////////////
void Analyser::func_param_list()
{
    func_param();
    while(peek().getTokenType() == TokenType::COMMA)
    {
        next();
        func_param();
    }
}

/////////////////////
/// \brief stmt -> expr_stmt
///        | decl_stmt
///        | if_stmt
///        | while_stmt
///        | break_stmt
///        | continue_stmt
///        | return_stmt
///        | block_stmt
///        | empty_stmt
/////////////////////
void Analyser::stmt()
{
    auto type = peek().getTokenType();
    if(type == TokenType::LET_KW || type == TokenType::CONST_KW) decl_stmt();
    else if(type == TokenType::IF_KW) if_stmt();
    else if(type == TokenType::WHILE_KW) while_stmt();
    else if(type == TokenType::BREAK_KW) break_stmt();
    else if(type == TokenType::CONTINUE_KW) continue_stmt();
    else if(type == TokenType::RETURN_KW) return_stmt();
    else if(type == TokenType::L_BRACE) block_stmt();
    else if(type == TokenType::SEMICOLON) empty_stmt();
    else expr();
}

////////////////////
/// \brief empty_stmt -> ';'
////////////////////
void Analyser::empty_stmt()
{
    expect(TokenType::SEMICOLON);
}

//////////////////
/// \brief block_stmt -> '{' stmt* '}'
//////////////////
void Analyser::block_stmt()
{
    expect(TokenType::L_BRACE);
    while(has_next() && peek().getTokenType() != TokenType::R_BRACE)
        stmt();
    expect(TokenType::R_BRACE);
}

//////////////////
/// \brief return_stmt -> 'return' expr? ';'
//////////////////
void Analyser::return_stmt()
{
    expect(TokenType::RETURN_KW);
    expr();
    expect(TokenType::SEMICOLON);
}

////////////////////
/// \brief continue_stmt -> 'continue' ';'
////////////////////
void Analyser::continue_stmt()
{
    expect(TokenType::CONTINUE_KW);
    expect(TokenType::SEMICOLON);
}

/////////////////
/// \brief break_stmt -> 'break' ';'
/////////////////
void Analyser::break_stmt()
{
    expect(TokenType::BREAK_KW);
    expect(TokenType::SEMICOLON);
}

//////////////////
/// \brief while_stmt -> 'while' expr block_stmt
//////////////////
void Analyser::while_stmt()
{
    expect(TokenType::WHILE_KW);
    expr();
    block_stmt();
}

///////////////
/// \brief if_stmt -> 'if' expr block_stmt ('else' 'if' expr block_stmt)* ('else' block_stmt)?
///////////////
void Analyser::if_stmt()
{
    expect(TokenType::IF_KW);
    expr();
    block_stmt();

}

//////////////////////
/// \brief const_decl_stmt -> 'const' IDENT ':' ty '=' expr ';'
//////////////////////
void Analyser::const_decl_stmt()
{
    expect(TokenType::CONST_KW);
    expect(TokenType::IDENT);
    expect(TokenType::COLON);
    expect(TokenType::TY);
    expect(TokenType::ASSIGN);
    expr();
    expect(TokenType::SEMICOLON);
}

//////////////////////
/// \brief let_decl_stmt -> 'let' IDENT ':' ty ('=' expr)? ';'
//////////////////////
void Analyser::let_decl_stmt()
{
    expect(TokenType::LET_KW);
    expect(TokenType::IDENT);
    expect(TokenType::TY);
    if(peek().getTokenType() == TokenType::ASSIGN)
    {
        next();
        expr();
    }
    expect(TokenType::SEMICOLON);
}

/////////////////
/// \brief expr_stmt -> expr ';'
/////////////////
void Analyser::expr_stmt()
{
    expr();
    expect(TokenType::SEMICOLON);
}

////////////////
/// \brief expr -> operator_expr
///                | negate_expr
///                | assign_expr
///                | as_expr
///                | call_expr
///                | literal_expr
///                | ident_expr
///                | group_expr
/// \brief 这里采用算符优先文法。
///        将所有可能的表达式的终结符放入一个缓存
///        可能表示表达式的终结符有以下这些
///        符号:         +、-、*、/、==、!=、<、>、<=、>=、=
///        字面值常量:   INT_LITERAL、DOUBLE_LITERAL、CHAR_LITERAL、STRING_LITERAL
///        标识符:       IDENT
///        括号:         ()
///        逗号:         ,
////////////////
bool isExpressionTermination(TokenType type)
{
    return type == TokenType::PLUS || type == TokenType::MINUS || type == TokenType::MUL ||
        type == TokenType::COMMA || type == TokenType::DIV || type == TokenType::EQ || type == TokenType::NEQ
        || type == TokenType::GT || type == TokenType::GE || type == TokenType::LT || type == TokenType::LE
        || type == TokenType::ASSIGN || type == TokenType::IDENT || type == TokenType::UINT_LITERAL || type == TokenType::CHAR_LITERAL
        || type == TokenType::STRING_LITERAL || type == TokenType::DOUBLE_LITERAL || type == TokenType::L_PAREN
        || type == TokenType::R_PAREN;
}
void Analyser::expr()
{
    // auto t = peek().getTokenType();
    // if(t == TokenType::MINUS) negate_expr();
    // else if(t == TokenType::L_PAREN) group_expr();
    while(has_next() && isExpressionTermination(peek().getTokenType()))
        next();
}

/////////////////////
/// \brief operator_expr -> expr [+-*/==!=<><=>=] expr
/////////////////////
void Analyser::operator_expr()
{

}

/////////////////////
/// \brief negate_expr -> '-' expr
/////////////////////
void Analyser::negate_expr()
{
    expect(TokenType::MINUS);
    expr();
}

////////////////////
/// \brief assign_expr -> ident '=' expr
////////////////////
void Analyser::assign_expr()
{
    expect(TokenType::IDENT);
    expect(TokenType::ASSIGN);
    expr();
}

///////////////////
/// \brief as_expr -> expr 'as' ty
///////////////////
void Analyser::as_expr()
{
    expr();
    expect(TokenType::AS_KW);
    expect(TokenType::TY);
}

//////////////////////
/// \brief call_param_list -> expr (',' expr)*
//////////////////////
void Analyser::call_param_list()
{
    expr();
    while(peek().getTokenType() == TokenType::COMMA)
    {
        next();
        expr();
    }
}

//////////////////////
/// \brief call_expr -> IDENT '(' call_param_list? ')'
//////////////////////
void Analyser::call_expr()
{
    expect(TokenType::IDENT);
    expect(TokenType::L_PAREN);
    call_param_list();
    expect(TokenType::R_PAREN);
}

/////////////////////
/// \brief UINT_LITERAL | DOUBLE_LITERAL | STRING_LITERAL | CHAR_LITERAL
/////////////////////
void Analyser::literal_expr()
{
    auto t = peek().getTokenType();
    if(t != TokenType::UINT_LITERAL && t != TokenType::STRING_LITERAL &&
        t != TokenType::DOUBLE_LITERAL && t != TokenType::CHAR_LITERAL)
        throw std::exception();
    next();
}

/////////////////////
/// \brief ident_expr -> ident
/////////////////////
void Analyser::ident_expr()
{
    expect(TokenType::IDENT);
}

////////////////////
/// \brief '(' expr ')'
////////////////////
void Analyser::group_expr()
{
    expect(TokenType::L_PAREN);
    expr();
    expect(TokenType::R_PAREN);
}

////////////////////
/// \brief ident
/////////////////////
void Analyser::l_expr()
{
    expect(TokenType::IDENT);
}

const Token & Analyser::peek() const
{
    if(index+1 >= tokens.size()) throw Error(ErrorCode::ErrAssignToConstant,Position(0,0));
    return tokens[index+1];
}

const Token & Analyser::next()
{
    if(index+1 >= tokens.size()) throw Error(ErrorCode::ErrAssignToConstant,Position(0,0));
    index++;
    return tokens[index];
}
const Token & Analyser::current() const
{
    return tokens[index];
}
const Token & Analyser::previous() 
{
    if(index <= 0) throw Error(ErrorCode::ErrAssignToConstant,Position(0,0));
    index--;
    return tokens[index];
}
void Analyser::expect(TokenType type)
{
    if(index+1 >= tokens.size()) throw Error(ErrorCode::ErrAssignToConstant,Position(0,0));
    index++;
    if(tokens[index].getTokenType() != type)
        throw Error(ErrorCode::ErrAssignToConstant,Position(0,0));
    
}