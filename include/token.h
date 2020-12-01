#ifndef TOKEN_H_
#define TOKEN_H_
#include "position.h"
#include <iostream>

/////////////////////////
/// \brief 表示token类型的枚举
/// \note 其中，UINT_LITERAL的value为long long
///       DOUBLE_LITERAL的value为double
///       CHAR_LITERAL的value为char
///       STRING_LITERAL、IDENT、COMMENT的value为string
///       剩下的没有value
/////////////////////////
enum class TokenType : int
{
    // 关键字
    FN_KW, // -> 'fn' 表示函数
    LET_KW, // -> 'let'
    CONST_KW, // -> 'const'
    AS_KW, // -> 'as'
    WHILE_KW, // -> 'while'
    IF_KW, // -> 'if'
    ELSE_KW, // -> 'else'
    RETURN_KW, // -> 'return'

    BREAK_KW, // -> 'break'
    CONTINUE_KW, // -> 'continue'

    // 字面值
    UINT_LITERAL, // 无符号整数字面值
    DOUBLE_LITERAL, // 浮点数字面值
    STRING_LITERAL, // 字符串字面值
    CHAR_LITERAL, // 字符字面值
    IDENT, // 标识符
    PLUS, // -> '+'
    MINUS, // -> '-'
    MUL, // -> '*'
    DIV, // -> '/'
    ASSIGN, // -> '='
    EQ, // -> '=='
    NEQ, // -> '!='
    LT, // -> '<'
    GT, // -> '>'
    LE, // -> '<='
    GE, // -> '>='
    L_PAREN, // -> '('
    R_PAREN, // -> ')'
    L_BRACE, // -> '{'
    R_BRACE, // -> '}'
    ARROW, // -> '->'
    COMMA, // -> ','
    COLON, // -> ':'
    SEMICOLON, // -> ';'
    COMMENT, // '//' ..... '\n'

    // 类型
    TY
};

class Token
{
private:
    ////////////////
    /// \brief token的类型
    ////////////////
    TokenType type;

    //////////////////
    /// \brief 表示token值的union，
    ///        值的类型可以是字符串，字符，和整数以及浮点数
    //////////////////
    std::string value;

    ///////////////
    /// \brief 该token开始的位置
    ///////////////
    Position start;

    //////////////////
    /// \brief 该token结束的位置
    //////////////////
    Position end;
    
public:
    /////////////////////////////
    /// \brief 默认构造函数，将value.s置为nullptr
    /////////////////////////////
    Token() = default;

    ///////////////////////////////////////
    /// \brief token的值为long long的时候的构造函数
    /// \param type token的类型
    /// \param value token的值
    /// \param start 这个token的开始位置
    /// \param end 这个token的结束位置
    ///////////////////////////////////////
    Token(const TokenType type,const std::string & value,const Position & start,const Position & end);
    
    ////////////////////////////////////////
    /// \brief token没有值的时候的构造函数
    /// \param type token的类型
    /// \param start 这个token的开始位置
    /// \param end 这个token的结束位置
    ///////////////////////////////////////
    Token(const TokenType type,const Position & start,const Position & end);
    
    ///////////////////////
    /// \brief 析构函数，主要用于字符串空间的释放
    ///////////////////////
    ~Token() = default;

    //////////////////////
    /// \brief 复制构造函数
    /// \param token 
    //////////////////////
    Token(const Token & token) = default;

    //////////////////////
    /// \brief 移动构造函数
    /// \param token 
    //////////////////////
    Token(Token && token) = default;

    /////////////////////
    /// \brief 赋值运算符
    /// \param token
    /////////////////////
    Token & operator=(const Token & token) = default;

    ////////////////////////
    /// \brief 移动赋值
    /// \param token
    ////////////////////////
    Token & operator=(Token && token) = default;

    TokenType getTokenType() const { return type;}
    const std::string & getValue() const { return value; }
    friend std::ostream & operator<<(std::ostream & os,const Token & token);
};
#endif // TOKEN_H_