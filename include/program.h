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
    // VaribleTable globals;
    FunctionTable functions;
    Function _start;
public:
    Program(unsigned int magic=0x72303b3e,unsigned int version = 0x1): 
        magic(magic),version(version),_start("_start") {}
    ~Program() = default;
    unsigned int getMagic() const { return magic; }
    unsigned int getVersion() const { return version; }
    // VaribleTable & getGlobals() { return _start.getVaribleTable(); }
    FunctionTable & getFunctions() { return functions; }
    friend std::ostream & operator<<(std::ostream & os, const Program & pg)
    {
        write(os,(void *)&pg.magic,sizeof(pg.magic));
        write(os,(void *)&pg.version,sizeof(pg.version));
        os << pg._start.getVaribleTable();
        os << pg._start;
        os << pg.functions;
        return os;
    }
};


#endif // PROGRAM_H_