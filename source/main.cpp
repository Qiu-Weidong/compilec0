#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"
// char buf[10006];
int main(int argc, char *argv[])
{
    Tokenizer tokenizer;
    std::fstream fout;
    fout.open(argv[4]);
    fout << ".." << std::endl;
    try
    {
        tokenizer.analyse(argv[2]);
        auto tokens = tokenizer.getTokens();
        for (const auto &token : tokens)
        {
            std::cout << token;
        }
    }
    catch (Error &err)
    {
        // std::cerr << err << std::endl;
        return -1;
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what() << '\n';
        return -1;
    }

}