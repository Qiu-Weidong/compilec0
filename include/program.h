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
    void init() ;

    Function & get_start() { return _start; }

    friend std::ostream & operator<<(std::ostream & os, const Program & pg);
};


#endif // PROGRAM_H_