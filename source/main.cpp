#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"

int main(int argc, char *argv[])
{
    Tokenizer tokenizer;
    try
    {
        tokenizer.analyse(argv[2]);
        auto tokens = tokenizer.getTokens();
        for (const auto &token : tokens)
        {
            std::cout << token << std::endl;
        }
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