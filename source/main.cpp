#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"
#include "analyser.h"
#include "varible.h"
#include "program.h"

////////////////////
/// \brief 全局变量的符号表
////////////////////
// VaribleTable globals;

Program program;

int main(int argc, char *argv[])
{
    Tokenizer tokenizer;
    try
    {
        tokenizer.analyse(argv[2]);
        const auto tokens = tokenizer.getTokens();
        // 错误原因，构造函数没有按引用传递，于是analyser中的引用指向了一个临时变量
        // 这个变量在之后就消失了。
        Analyser analyser(tokens);
        // 进行语法分析并生成指令
        analyser.program();
        // 输出到二进制文件
        std::ofstream fout;
        fout.open(argv[4],std::ios::out|std::ios::binary);
        if(!fout.is_open()) return -1;
        fout << program;
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