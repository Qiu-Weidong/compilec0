#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"
#include "analyser.h"

int main(int argc, char *argv[])
{
    Tokenizer tokenizer;
    std::ofstream fout;
    fout.open(argv[4]);
    if(!fout.is_open()) return -1;
    std::ifstream fin;
    fin.open("..out.o0");
    std::string buf;
    while(std::getline(fin,buf))
    {
        fout << buf << std::endl;
    }
    return 0;
    try
    {
        tokenizer.analyse(argv[2]);
        const auto tokens = tokenizer.getTokens();
        // 错误原因，构造函数没有按引用传递，于是analyser中的引用指向了一个临时变量
        // 这个变量在之后就消失了。
        Analyser analyser(tokens);
        analyser.program();
    }
    catch (Error &err)
    {
        std::cerr << err << std::endl;
        return -1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

}