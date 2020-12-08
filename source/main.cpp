#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "tokenizer.h"
#include "error.h"
#include "analyser.h"
#include "varible.h"
#include "program.h"

// 字符串字面值保存在这里
char * str[1000];
// 全局变量的偏移
unsigned int nextGlobalOffset;
Program program;
void init();
void free();
int main(int argc, char *argv[])
{
    init();
    Tokenizer tokenizer;
    try
    {
        tokenizer.analyse(argv[2]);
        const auto tokens = tokenizer.getTokens();
        Analyser analyser(tokens);
        // for(const auto & token : tokens) std::cout << token << std::endl;
        // 进行语法分析并生成指令
        analyser.program(program);
        // 输出到二进制文件
#ifndef DEBUG
        std::ofstream fout;
        fout.open(argv[4],std::ios::out|std::ios::binary);
        if(!fout.is_open()) return -1;
        fout << program;
#else
        std::cout << program << std::endl;
#endif //DEBUG
    }
    catch (Error &err)
    {
        std::cerr << err << std::endl;
        free();
        return -1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        free();
        return -1;
    }
    free();
    return 0;
}

void init()
{
    memset(str,0,sizeof(str));
    nextGlobalOffset = 0;
    program.init();
}
void 
free()
{
    for(int i=0;i<1000;i++)
    {
        if(str[i] != nullptr)
            delete [] str[i];
    }
}