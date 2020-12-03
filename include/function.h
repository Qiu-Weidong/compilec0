#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <string>
#include <vector>
#include "varible.h"
#include "instruction.h"

//////////////////
/// \brief 函数的声明
//////////////////
class FunctionDecl
{
private:
    /////////
    /// \brief 给函数分配一个fid
    /////////
    int fid;

    //////////////
    /// \brief 返回类型
    //////////////
    Type retType;

    ////////////////
    /// \brief 函数名
    ////////////////
    std::string name;

    ///////////////////////////
    /// \brief 参数列表，如果有返回值，那么第一个参数的address为返回值的偏移
    ///        没有返回值，那么第一个参数的address为0，以此类推
    ///////////////////////////
    std::vector<Varible> params;
    
public:
    FunctionDecl() = default;
    FunctionDecl(const FunctionDecl &) = default;
    FunctionDecl(FunctionDecl &&) = default;
    FunctionDecl & operator=(const FunctionDecl &) = default;
    FunctionDecl & operator=(FunctionDecl &&) = default;
    ~FunctionDecl() = default;

    FunctionDecl(const std::string & name,Type ret_type) : name(name),retType(ret_type) { params.clear(); }
    void addParam(const Varible & vb) ;
};

class Function
{
private:
    FunctionDecl decl;
    std::vector<Instruction> instructions;
    
};

#endif // FUNCTION_H_