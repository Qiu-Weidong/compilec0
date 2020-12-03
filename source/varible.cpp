#include "varible.h"
#include "error.h"

bool VaribleTable::isDeclared(const std::string &name)
{
    for (const auto &var : table)
    {
        if (var.getName() == name)
            return true;
    }
    return false;
}
const Varible &VaribleTable::get(const std::string &name, const Position &pos)
{
    for (const auto &var : table)
    {
        if (var.getName() == name)
            return var;
    }
    if (parent == nullptr)
        throw Error(ErrorCode::VaribleNotDecl, "varible \"" + name + "\" is not declared!", pos);
    return parent->get(name, pos);
}
void VaribleTable::insert(Varible &var, const Position &pos)
{
    if (isDeclared(var.getName()))
        throw Error(ErrorCode::DuplicateDecl, "varible \"" + var.getName() + "\" is duplicate declared!", pos);
    if (var.getKind() == Kind::PARAM)
        var.setAddress(param_offset++);
    else
        var.setAddress(loca_offset++);
    table.push_back(var);
}

std::ostream &operator<<(std::ostream &os, const VaribleTable &vr)
{
    int n = vr.table.size();
    write(os, (void *)&n, sizeof(n));
    for (const auto &var : vr.table)
        os << var;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Varible &var)
{
    char c = var.is_const ? 1 : 0;
    // 是否是常量
    write(os, (void *)&c, sizeof(c));
    // 变量的size
    write(os, (void *)&var.size, sizeof(var.size));
    long long t = 0;
    // 用0填充，在_start函数里面去完成赋值
    write(os, (void *)&t, var.size);
    return os;
}