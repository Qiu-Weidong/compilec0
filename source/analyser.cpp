#include "analyser.h"

const Token &Analyser::peek() const
{
    return tokens[index + 1];
}

const Token &Analyser::next()
{
    index++;
    return tokens[index];
}
const Token &Analyser::current() const
{
    return tokens[index];
}
const Token &Analyser::previous()
{
    if (index <= 0)
        throw Error(ErrorCode::NoToken, "there is no previous token!");
    index--;
    return tokens[index];
}
const Token & Analyser::expect(TokenType type)
{
    if (!has_next())
        throw Error(ErrorCode::NoToken, "there is no token anymore!", sentinel.getStart());
    index++;
    if (tokens[index].getTokenType() != type)
        throw Error(ErrorCode::ExpectFail, "expect fail", current().getStart());
    return tokens[index];
}