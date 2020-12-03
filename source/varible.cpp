#include "varible.h"
#include "error.h"

bool VaribleTable::isDeclared(const std::string & name)
{
    for(const auto & var : table)
    {
        if(var.getName() == name) return true;
    }
    return false;
}
const Varible & VaribleTable::get(const std::string & name,const Position & pos)
{
    for(const auto & var : table)
    {
        if(var.getName() == name) return var;
    }
    if(parent == nullptr) throw Error(ErrorCode::VaribleNotDecl,"varible \""+name+"\" is not declared!",pos);
    return parent->get(name,pos);
}
void VaribleTable::insert(const Varible & var,const Position & pos)
{
    if(isDeclared(var.getName())) throw Error(ErrorCode::DuplicateDecl,"varible \""+var.getName()+"\" is duplicate declared!",pos);
    table.push_back(var);
}