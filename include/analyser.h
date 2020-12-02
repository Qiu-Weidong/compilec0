#ifndef ANALYSER_H_
#define ANALYSER_H_
#include <vector>
#include "token.h"

/*
 * 分析
 * 终结符有如下这些: 38个，除去注释37个
 * 关键字:  FN_KW、LET_KW、CONST_KW、AS_KW、WHILE_KW、IF_KW、
 *          ELSE_KW、RETURN_KW 、BREAK_KW、CONTINUE_KW 
 * 字面量: UINT_LITERAL 、DOUBLE_LITERAL 、STRING_LITERAL 
 *          CHAR_LITERAL 
 * 类型: INT、DOUBLE、VOID
 * 符号: PLUS、MINUS、MUL、DIV、ASSIGN、EQ、NEQ、LT、GT、
 *       LE、GE、L_PAREN、R_PAREN、L_BRACE、R_BRACE、ARROW、
 *       COMMA、COLON、SEMICOLON
 * 注释: COMMENT
 * 标识符: IDENT
 * 
 * 非终结符 28个
 * <expr>、
 * <operator_expr>、<negate_expr>、<assign_expr>、
 * <as_expr>、<call_expr>、<literal_expr>、
 * <ident_expr>、<group_expr>、
 * 
 * <call_param_list>、
 * 
 * <stmt>、<expr_stmt>、<decl_stmt>、
 * <if_stmt>、<while_stmt>、<break_stmt>、
 * <continue_stmt>、<return_stmt>、
 * <block_stmt>、<empty_stmt>、
 * 
 * <let_decl_stmt>、<const_decl_stmt>、<decl_stmt>
 * 
 * <function_param>、<function_param_list>、<function>
 * <program>、<item>
*/
class Analyser
{
private:
    const std::vector<Token> & tokens;
    int index;
    Token sentinel;

    const Token & peek() const;
    const Token & next() ;
    const Token & current() const;
    const Token & previous();
    void expect(TokenType type) ;
    bool has_next() const 
    {
        if(index+1 >= tokens.size()) return false;
        return tokens[index+1].getTokenType() != TokenType::NONE;
    }
    bool has_previous() const { return index > 0; }

public:
    Analyser(const std::vector<Token> & _tokens) : tokens(_tokens) 
    { 
        index = 0; sentinel = _tokens.at(_tokens.size()-1); 
    }
    Analyser(const Analyser & ) = delete;
    Analyser(Analyser && ) = delete;
    ~Analyser() = default;
    Analyser & operator=(const Analyser &) = delete;
    Analyser & operator=(Analyser &&) = delete;

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

    /////////////////////////
    /// \brief stmt
    /////////////////////////
    void stmt();

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