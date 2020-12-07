#include <cstring>
#include "program.h"

extern unsigned int nextGlobalOffset;
extern char *str[1000];

void Program::init()
{
    _start.setFid(0);
    _start.setLocaSlots(0);
    _start.setParamSlots(0);
    _start.setReturnSlots(0);
    _start.setReturnType(Type::VOID);
    _start.setName("_start");

    Varible fn_name;
    fn_name.setConst(false);
    fn_name.setKind(Kind::GLOBAL);
    // 这是一个匿名的全局变量
    fn_name.setName("");
    fn_name.setType(Type::STRING);
    fn_name.setAddress(nextGlobalOffset++);
    fn_name.setSize(6);
    unsigned int addr = fn_name.getAddress();
    // 将函数名称保存到str当中
    str[addr] = new char[7];
    _start.setFname(addr);
    strcpy(str[addr], "_start");
    globals.insert(fn_name, Position(0, 0));
}

#ifndef DEBUG
std::ostream &operator<<(std::ostream &os, const Program &pg)
{
    write(os, (void *)&pg.magic, sizeof(pg.magic));
    write(os, (void *)&pg.version, sizeof(pg.version));
    os << pg.globals;
    int n = pg.functions.size() + 1;
    write(os,(void *)(&n),sizeof(n));
    os << pg._start;
    os << pg.functions;
    return os;
}
#else
std::ostream &operator<<(std::ostream &os, const Program &pg)
{
    os << "magic:  " << pg.magic << std::endl;
    os << "version:" << pg.version << std::endl;
    os << pg.globals << std::endl;
    os << pg._start << std::endl;
    os << pg.functions << std::endl;
    return os;
}
#endif //DEBUG