#ifndef ERROR_H_
#define ERROR_H_
#include <exception>
#include <iostream>
#include <string>
#include "position.h"

//////////////////////
/// \brief 所有的异常类型的枚举
//////////////////////
enum ErrorCode : int
{
    // IO错误
    // 没有输入文件
    NoInput,
    // 没有输出文件
    NoOutput,

    // 词法分析错误
    // 无效的浮点数常量
    InvalidDoubleLiteral,
    // 无效的整形常量
    InvalidUintLiteral,
    // 无效的字符串
    InvalidStringLiteral,
    // 无效的字符字面值
    InvalidCharLiteral,
    // 无效的标识符
    InvalidIdent,
    // 无效的符号
    InvalidSymbol,
    // 错误的注释
    BadComment,
    // 不可识别的字符
    UnknownChar,
    // 未知错误
    UnknownErr,

    // 语法分析的错误
    // 没有token了
    NoToken,
    InvalidItem,
    InvalidDeclare,
    ExpectFail,
    invalidStmt,
    VaribleNotDecl,
    DuplicateDecl,
    FunctionNotDecl,
    InvalidType,
    ConstMustBeInitial,
    TypeNotMatch,
    ConstValueChanged,
    InvalidAssign,
    ParamNotMatch,
    ParamNotEnough,
    InvalidCallList,
    BadExpr,
};

class Error : public std::exception
{
private:
    ///////////////
    /// \brief 错误码
    ///////////////
    ErrorCode code;

    ///////////////
    /// \brief 错误的位置
    ///////////////
    Position pos;

    ///////////////
    /// \brief 错误的消息
    ///////////////
    std::string msg;

public:
    Error(ErrorCode code, const std::string &msg, const Position &pos)
    {
        this->code = code;
        this->pos = pos;
        this->msg = msg;
    }
    Error(ErrorCode code,const std::string & msg) { this->code = code; this->msg = msg; }
    ~Error() = default;

    // virtual const char * what() const override {}
    ErrorCode getCode() const { return code; }
    Position getPosition() const { return pos; }
    const std::string & getMsg() const { return msg; }

    friend std::ostream &operator<<(std::ostream &os, const Error &err)
    {
        os << err.pos << " " << err.msg ;
        return os;
    }
};

#endif //ERROR_H_