#include <iostream>
#include <fstream>
#include <string>
#include "tokenizer.h"
#include "error.h"
#include "analyser.h"

int main(int argc, char *argv[])
{
    Tokenizer tokenizer;
    try
    {
        tokenizer.analyse(argv[2]);
        auto tokens = tokenizer.getTokens();
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