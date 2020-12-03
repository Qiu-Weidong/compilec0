#ifndef VARIBLE_H_
#define VARIBLE_H_
#include <string>
#include <vector>
#include "position.h"

////////////////
/// \brief 变量具体的类型
////////////////
enum class Type
{
    // 整形
    INT,
    // void型
    VOID,
    // 浮点型
    DOUBLE
};
///////////////
/// \brief 符号表的条目类型，
///////////////
enum class Kind
{
    // 参数
    PARAM,
    // 函数，可以不要，将函数放在另一张表当中
    FUNC,
    // 变量
    VAR
};

//////////////
/// \brief 变量
//////////////
class Varible
{
private :
    bool is_const;
    std::string name;
    Kind kind;
    Type type;
    int size;

    ///////////////////////////
    /// \brief 地址
    ///////////////////////////
    unsigned long long address;
public:
    Varible() = default;
    Varible(const Varible &) = default;
    Varible(Varible &&) = default;
    Varible & operator=(const Varible &) = default;
    Varible & operator=(Varible &&) = default;
    ~Varible() = default;
    Varible(const Type type,const std::string & name,unsigned long long address,bool is_const = false) 
    {
        this->type = type;
        this->name = name;
        this->address = address;
        this->is_const = is_const;
        if(type == Type::VOID) size = 0;
        else size = 8;
    }

    bool isConst() const { return is_const; }
    const std::string & getName() const { return name; }
    Type getType() const { return type; }
    int getSize() const { return size; }
    unsigned long long getAddress() const { return address; }
};

//////////////////
/// \brief 变量的符号表
//////////////////
class VaribleTable
{
private:
    VaribleTable * parent;
    std::vector<Varible> table;
    // 可以假想存在两个栈，返回值和参数放在一个栈中，局部变量放在另一个
    int param_offset; // 下一个参数的偏移
    int loca_offset; // 下一个局部变量的偏移
public:
    VaribleTable(VaribleTable * parent = nullptr,int param_offset=0,int loca_offset=0) 
        : parent(parent),param_offset(param_offset),loca_offset(loca_offset) {}
    bool isDeclared(const std::string & var_name);
    void insert(const Varible & var,const Position & pos);
    const Varible & get(const std::string & var_name,const Position & pos);
};
#endif // VARIBLE_H_