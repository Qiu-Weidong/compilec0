#ifndef ANALYSER_H_
#define ANALYSER_H_
#include <vector>
#include "token.h"
#include "error.h"


class Analyser
{
private:
    const std::vector<Token> & tokens;
    int index;
    Token sentinel;

    const Token & peek() const;
    const Token & next() ;
    const Token & current() const;
    const Token & previous();
    const Token & expect(TokenType type) ;
    bool has_next() const 
    {
        if(index+1 >= tokens.size()) return false;
        return tokens[index+1].getTokenType() != TokenType::NONE;
    }
    bool has_previous() const { return index > 0; }
public:
    Analyser(const std::vector<Token> & _tokens) : tokens(_tokens) 
    { 
        index = 0; sentinel = _tokens.at(_tokens.size()-1); 
    }
    Analyser(const Analyser & ) = delete;
    Analyser(Analyser && ) = delete;
    ~Analyser() = default;
    Analyser & operator=(const Analyser &) = delete;
    Analyser & operator=(Analyser &&) = delete;


};



#endif // ANALYSER_H_