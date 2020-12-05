#include "function.h"

std::ostream &operator<<(std::ostream &os, const Function &f)
{
    write(os, (void *)&f.fid, sizeof(f.fid));
    write(os, (void *)&f.return_slots, sizeof(f.return_slots));
    write(os, (void *)&f.param_slots, sizeof(f.param_slots));
    write(os, (void *)&f.loca_slots, sizeof(f.loca_slots));
    int n = f.instructions.size();
    write(os, (void *)&n, sizeof(n));
    for (const auto &instruction : f.instructions)
        os << instruction;
    return os;
}

bool FunctionTable::isDeclared(const std::string &name)
{
    for (const auto &fn : functions)
    {
        if (fn.getName() == name)
            return true;
    }
    return false;
}
int FunctionTable::insert(Function &fn, const Position &pos)
{
    if (isDeclared(fn.getName()))
        throw Error(ErrorCode::DuplicateDecl, "function \"" + fn.getName() + "\" is duplicate declared!", pos);
    fn.setFid(next_fid++);
    functions.push_back(fn);
    return fn.getFid();
}

Function &FunctionTable::get(const std::string &name, const Position &pos)
{
    for (auto &fn : functions)
    {
        if (fn.getName() == name)
            return fn;
    }
    throw Error(ErrorCode::FunctionNotDecl, "function \"" + name + "\" is not declared!", pos);
}

Function & FunctionTable::get(int fid,const Position & pos)
{
    for(auto & fn : functions)
    {
        if(fn.getFid() == fid) return fn;
    }
    throw Error(ErrorCode::FunctionNotDecl,"function "+std::to_string(fid)+" is not declared!",pos);
}
std::ostream &operator<<(std::ostream &os, const FunctionTable &ft)
{
    int n = ft.functions.size();
    write(os, (void *)&n, sizeof(n));
    for (const auto &fn : ft.functions)
        os << fn;
    return os;
}