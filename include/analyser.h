#ifndef ANALYSER_H_
#define ANALYSER_H_
#include <vector>
#include "token.h"
#include "error.h"
#include "program.h"
#include "function.h"
#include "varible.h"


class Analyser
{
private:

#ifdef DEBUG
    TokenType cur;
    TokenType follow;

    std::string curValue;
    std::string followValue;
#endif // DEBUG


    const std::vector<Token> & tokens;
    int index;
    Token sentinel;

    const Token & peek() const;
    const Token & next() ;
    const Token & current() const;
    const Token & previous();
    const Token & expect(TokenType type) ;
    bool has_next() const ;
    bool has_previous() const;

    bool isExpressionTermination(TokenType type);

    /////////////////////////////////////////
    /// \brief 表达式
    /// \param vt 需要在符号表当中查找标识符
    /// \param ft 函数表，用于查找函数
    /// \param fn 将指令放入fn函数中
    /// \exception 如果未定义，则抛出异常
    /// expr -> 
    /////////////////////////////////////////
    Type expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type operator_expr(Type t,TokenType op, Function & fn);
    Type negate_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type assign_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type as_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type call_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type literal_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type ident_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);
    Type group_expr(VaribleTable & vt,FunctionTable & ft, Function & fn);


    ///////////////////////////////////////////////////
    /// \brief 分析函数 function -> 'fn' IDENT '(' function_param_list? ')' '->' ty block_stmt
    /// \param parent 函数的父级符号表
    /// \param ft 函数列表，需要在ft中查询已经定义的函数，同时需要将识别到的函数填入ft中
    ///////////////////////////////////////////////////
    void func(VaribleTable & parent,FunctionTable & ft);
    Varible func_param();
    std::vector<Varible> func_param_list();





    
    /////////////////////////////////////////////////////////////
    /// \brief 分析语句
    /// \param vt 符号表
    /// \param ft 函数表
    /// \param fn 函数
    /////////////////////////////////////////////////////////////
    void stmt(VaribleTable & vt,FunctionTable & ft,Function & fn,int while_st = -1);
    void block_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn,int while_st = -1);
    void expr_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn);
    void if_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn,int while_st = -1);
    void while_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn);
    void break_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn);
    void continue_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn);
    void return_stmt(VaribleTable & vt,FunctionTable & ft,Function & fn);
    void empty_stmt(); // 可以不要
    ////////////////
    /// \brief 变量声明语句
    /// \param vt 声明的变量需要填入的符号表
    /// \param fn 如果初始化，则将指令放入fn函数中
    /// \exception 如果重复定义，则抛出异常
    /// decl_stmt -> [let,const] ident : ty (= expr)? ;
    ////////////////
    void decl_stmt(VaribleTable & vt, FunctionTable & ft,Function & fn);

public:
    Analyser(const std::vector<Token> & _tokens);
    Analyser(const Analyser & ) = delete;
    Analyser(Analyser && ) = delete;
    ~Analyser() = default;
    Analyser & operator=(const Analyser &) = delete;
    Analyser & operator=(Analyser &&) = delete;

    //////////////////////////
    /// \brief 语法分析构造程序
    /// \param pg 构造的程序
    //////////////////////////
    void program(Program & pg);

};



#endif // ANALYSER_H_