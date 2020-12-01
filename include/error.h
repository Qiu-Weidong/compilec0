#ifndef ERROR_H_
#define ERROR_H_
#include <exception>
#include <iostream>
#include "position.h"

//////////////////////
/// \brief 所有的异常类型的枚举
//////////////////////
enum ErrorCode : int
{
    ErrNoError, // Should be only used internally.
    ErrStreamError,
    ErrEOF,
    ErrInvalidInput,
    ErrInvalidIdentifier,
    ErrIntegerOverflow, // int32_t overflow.
    ErrNoBegin,
    ErrNoEnd,
    ErrNeedIdentifier,
    ErrConstantNeedValue,
    ErrNoSemicolon,
    ErrInvalidVariableDeclaration,
    ErrIncompleteExpression,
    ErrNotDeclared,
    ErrAssignToConstant,
    ErrDuplicateDeclaration,
    ErrNotInitialized,
    ErrInvalidAssignment,
    ErrInvalidPrint,
    ErrNoFile
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
public:
    Error(ErrorCode code , Position pos) { this->code = code; this->pos = pos; }
    ~Error() = default;

    // virtual const char * what() const override {}
    ErrorCode getCode() const { return code ;}
    Position getPosition() const  { return pos; }

    friend std::ostream & operator<<(std::ostream & os,const Error & err) {
        os << "row:" << err.pos.row << ",col:" << err.pos.col << "error!" << std::endl ;
        return os;
    }
};

#endif //ERROR_H_