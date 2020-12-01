#include "analyser.h"
#include "token.h"
#include "error.h"

//////////////
/// \brief program -> item*
//////////////
void Analyser::program()
{

}

//////////////////
/// \brief item -> function | decl_stmt
//////////////////
void Analyser::item()
{

}

/////////////////
/// \brief decl_stmt -> let_decl_stmt | const_decl_stmt
/////////////////
void Analyser::decl_stmt()
{

}

//////////////////
/// \brief function -> 'fn' IDENT '(' function_param_list? ')' '->' ty block_stmt
//////////////////
void Analyser::func()
{

}

/////////////////////
/// \brief function_param -> 'const'? IDENT ':' ty
/////////////////////
void Analyser::func_param()
{

}

///////////////////////
/// \brief function_param_list -> function_param (',' function_param)*
///////////////////////
void Analyser::func_param_list()
{

}

////////////////////
/// \brief empty_stmt -> ';'
////////////////////
void Analyser::empty_stmt()
{

}

//////////////////
/// \brief block_stmt -> '{' stmt* '}'
//////////////////
void Analyser::block_stmt()
{

}

//////////////////
/// \brief return_stmt -> 'return' expr? ';'
//////////////////
void Analyser::return_stmt()
{

}

////////////////////
/// \brief continue_stmt -> 'continue' ';'
////////////////////
void Analyser::continue_stmt()
{

}

/////////////////
/// \brief break_stmt -> 'break' ';'
/////////////////
void Analyser::break_stmt()
{

}

//////////////////
/// \brief while_stmt -> 'while' expr block_stmt
//////////////////
void Analyser::while_stmt()
{

}

///////////////
/// \brief if_stmt -> 'if' expr block_stmt ('else' 'if' expr block_stmt)* ('else' block_stmt)?
///////////////
void Analyser::if_stmt()
{

}

//////////////////////
/// \brief const_decl_stmt -> 'const' IDENT ':' ty '=' expr ';'
//////////////////////
void Analyser::const_decl_stmt()
{

}

//////////////////////
/// \brief let_decl_stmt -> 'let' IDENT ':' ty ('=' expr)? ';'
//////////////////////
void Analyser::let_decl_stmt()
{

}

/////////////////
/// \brief expr_stmt -> expr ';'
/////////////////
void Analyser::expr_stmt()
{

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
////////////////
void Analyser::expr()
{

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

}

////////////////////
/// \brief assign_expr -> ident '=' expr
////////////////////
void Analyser::assign_expr()
{

}

///////////////////
/// \brief as_expr -> expr 'as' ty
///////////////////
void Analyser::as_expr()
{

}

//////////////////////
/// \brief call_param_list -> expr (',' expr)*
//////////////////////
void Analyser::call_param_list()
{

}

//////////////////////
/// \brief call_expr -> IDENT '(' call_param_list? ')'
//////////////////////
void Analyser::call_expr()
{

}

/////////////////////
/// \brief UINT_LITERAL | DOUBLE_LITERAL | STRING_LITERAL | CHAR_LITERAL
/////////////////////
void Analyser::literal_expr()
{

}

/////////////////////
/// \brief ident_expr -> ident
/////////////////////
void Analyser::ident_expr()
{

}

////////////////////
/// \brief '(' expr ')'
////////////////////
void Analyser::group_expr()
{

}

////////////////////
/// \brief ident
/////////////////////
void Analyser::l_expr()
{

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