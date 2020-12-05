#ifndef PROGRAM_H_
#define PROGRAM_H_
#include <fstream>
#include "output.h"
#include "varible.h"
#include "function.h"

class Program
{
private:
    unsigned int magic;
    unsigned int version;
    VaribleTable globals;
    FunctionTable functions;
    Function _start;
public:
    Program(unsigned int magic=0x72303b3e,unsigned int version = 0x1): 
        magic(magic),version(version) {  }
    ~Program() = default;

    unsigned int getMagic() const { return magic; }
    unsigned int getVersion() const { return version; }
    VaribleTable & getGlobals() { return globals; }
    FunctionTable & getFunctions() { return functions; }

    Function & getFunction(const std::string & name,const Position & pos) { 
        return functions.get(name,pos);
    }
    const Varible & getGlobal(const std::string & name,const Position & pos) { return globals.get(name,pos);}
    void addFunction(Function & fn,const Position & pos) { functions.insert(fn,pos); }
    void addGlobal(Varible & var, const Position & pos) { globals.insert(var,pos); } 

    /////////////
    /// \brief 初始化_start函数
    /////////////
    void init() 
    {
        _start.setFid(0);
        _start.setLocaSlots(0);
        _start.setParamSlots(0);
        _start.setReturnSlots(0);
        _start.setReturnType(Type::VOID);
        _start.setName("_start");
    }

    Function & get_start() { return _start; }

#ifndef DEBUG
    friend std::ostream & operator<<(std::ostream & os, const Program & pg)
    {
        write(os,(void *)&pg.magic,sizeof(pg.magic));
        write(os,(void *)&pg.version,sizeof(pg.version));
        os << pg.globals;
        os << pg.functions;
        return os;
    }
#else 
    friend std::ostream & operator<<(std::ostream & os,const Program & pg)
    {
        os << "magic:  " << pg.magic << std::endl;
        os << "version:" <<pg.version << std::endl;
        os << pg.globals << std::endl;
        os << pg._start << std::endl;
        os << pg.functions << std::endl;
        return os;
    }
#endif //DEBUG
};


#endif // PROGRAM_H_