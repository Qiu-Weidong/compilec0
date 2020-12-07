#include <cassert>
#include "varible.h"
#include "error.h"

extern char * str[1000];
bool VaribleTable::isDeclared(const std::string &name)
{
    if(name == "") return false; // 匿名变量不认为重名
    for (const auto &var : table)
    {
        if (var.getName() == name)
            return true;
    }
    return false;
}
const Varible &VaribleTable::get(const std::string &name, const Position &pos)
{
    if(name == "") throw Error(ErrorCode::VaribleNotDecl, "you can\'t get a anoymous varible!", pos);
    for (const auto &var : table)
    {
        if (var.getName() == name)
            return var;
    }
    if (parent == nullptr)
        throw Error(ErrorCode::VaribleNotDecl, "varible \"" + name + "\" is not declared!", pos);
    return parent->get(name, pos);
}
void VaribleTable::insert(const Varible &var, const Position &pos)
{
    if (isDeclared(var.getName()))
        throw Error(ErrorCode::DuplicateDecl, "varible \"" + var.getName() + "\" is duplicate declared!", pos);
    table.push_back(var);
}

#ifndef DEBUG
std::ostream &operator<<(std::ostream &os, const VaribleTable &vt)
{
    int n = vt.table.size();
    write(os, (void *)&n, sizeof(n));
    for (const auto &var : vt.table)
        os << var;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Varible &var)
{
    // 只有全局变量才需要输出
    assert(var.kind == Kind::GLOBAL);
    char c = var.is_const ? 1 : 0;
    // 是否是常量
    write(os, (void *)&c, sizeof(c));
    // 变量的size
    write(os, (void *)&var.size, sizeof(var.size));
    if (var.type == Type::STRING)
    {
        unsigned int addr = var.getAddress();
        assert(str[addr]!=nullptr);
        for(int i=0;i<var.size;i++) write(os,(void *)(str[addr]+i),sizeof(char));
    }
    else
    {
        long long t = 0;
        // 用0填充，在_start函数里面去完成赋值
        write(os, (void *)&t, var.size);
    }
    return os;
}
#else
std::ostream & operator<<(std::ostream & os,const Varible & var)
{
    assert(var.kind == Kind::GLOBAL);
    os << "is const: " << var.is_const << std::endl;
    os << "size:     " << var.size << std::endl;
    if(var.type == Type::STRING) 
    {
        os << "string: " << str[var.size] << std::endl;
    }
    else {
        for(int i=0;i<var.size;i++)
            os << "00 " ;
        os << std::endl;
    }
    return os;

}

std::ostream & operator<<(std::ostream & os,const VaribleTable & vt)
{
    os << "global counts: "<< vt.table.size();
    for(const auto & var : vt.table) os << var;
    return os;
}
#endif // DEBUG