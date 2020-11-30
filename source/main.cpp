#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"

int main(int argc, char *argv[])
{
    printf("just for test!\n");
    Tokenizer tokenizer;
    // std::ofstream fout;
    try
    {
        tokenizer.analyse(argv[1]);
        auto tokens = tokenizer.getTokens();
        // fout.open(argv[2]);
        for (const auto &token : tokens)
        {
            std::cout << token;
            // fout << token;
        }
    }
    catch (Error &err)
    {
        std::cerr << err << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}