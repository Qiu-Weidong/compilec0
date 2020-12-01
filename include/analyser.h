#ifndef ANALYSER_H_
#define ANALYSER_H_
#include <vector>
#include "token.h"

class Analyser
{
private:
    const std::vector<Token> & tokens;
    int index;
    const Token & peek() const;
    const Token & next() ;
    const Token & current() const;
    const Token & previous();
    void expect(TokenType type) ;
    bool has_next() const { return index < tokens.size(); }
    bool has_previous() const { return index > 0; }

public:
    //////////////
    /// \brief program -> item*
    //////////////
    void program();

    //////////////////
    /// \brief item -> function | decl_stmt
    //////////////////
    void item();

    /////////////////
    /// \brief decl_stmt -> let_decl_stmt | const_decl_stmt
    /////////////////
    void decl_stmt();

    //////////////////
    /// \brief function -> 'fn' IDENT '(' function_param_list? ')' '->' ty block_stmt
    //////////////////
    void func();

    /////////////////////
    /// \brief function_param -> 'const'? IDENT ':' ty
    /////////////////////
    void func_param();

    ///////////////////////
    /// \brief function_param_list -> function_param (',' function_param)*
    ///////////////////////
    void func_param_list();

    ////////////////////
    /// \brief empty_stmt -> ';'
    ////////////////////
    void empty_stmt();

    //////////////////
    /// \brief block_stmt -> '{' stmt* '}'
    //////////////////
    void block_stmt();

    //////////////////
    /// \brief return_stmt -> 'return' expr? ';'
    //////////////////
    void return_stmt();

    ////////////////////
    /// \brief continue_stmt -> 'continue' ';'
    ////////////////////
    void continue_stmt();

    /////////////////
    /// \brief break_stmt -> 'break' ';'
    /////////////////
    void break_stmt();

    //////////////////
    /// \brief while_stmt -> 'while' expr block_stmt
    //////////////////
    void while_stmt();

    ///////////////
    /// \brief if_stmt -> 'if' expr block_stmt ('else' 'if' expr block_stmt)* ('else' block_stmt)?
    ///////////////
    void if_stmt();

    //////////////////////
    /// \brief const_decl_stmt -> 'const' IDENT ':' ty '=' expr ';'
    //////////////////////
    void const_decl_stmt();

    //////////////////////
    /// \brief let_decl_stmt -> 'let' IDENT ':' ty ('=' expr)? ';'
    //////////////////////
    void let_decl_stmt();

    /////////////////
    /// \brief expr_stmt -> expr ';'
    /////////////////
    void expr_stmt();

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
    void expr();

    /////////////////////
    /// \brief operator_expr -> expr [+-*/==!=<><=>=] expr
    /////////////////////
    void operator_expr();

    /////////////////////
    /// \brief negate_expr -> '-' expr
    /////////////////////
    void negate_expr();

    ////////////////////
    /// \brief assign_expr -> ident '=' expr
    ////////////////////
    void assign_expr();

    ///////////////////
    /// \brief as_expr -> expr 'as' ty
    ///////////////////
    void as_expr();

    //////////////////////
    /// \brief call_param_list -> expr (',' expr)*
    //////////////////////
    void call_param_list();

    //////////////////////
    /// \brief call_expr -> IDENT '(' call_param_list? ')'
    //////////////////////
    void call_expr();

    /////////////////////
    /// \brief UINT_LITERAL | DOUBLE_LITERAL | STRING_LITERAL | CHAR_LITERAL
    /////////////////////
    void literal_expr();

    /////////////////////
    /// \brief ident_expr -> ident
    /////////////////////
    void ident_expr();

    ////////////////////
    /// \brief '(' expr ')'
    ////////////////////
    void group_expr();

    ////////////////////
    /// \brief ident
    /////////////////////
    void l_expr();

};
#endif // ANALYSER_H_