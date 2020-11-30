#include "tokenizer.h"
#include "error.h"
#include <cctype>
#include <cstdlib>
void Tokenizer::analyse(const char *file_name)
{
    fin.open(file_name);
    if (!fin.is_open())
        throw Error(ErrorCode::ErrNoFile, position(0, 0));
    currentChar = nextChar = '\0';
    // nextChar = fin.get();
    buffer.clear();
    currentState = DFAState::INITIAL_STATE;
    currentPos = position(-1,-1);
    nextPos = position(0,-1);
    getNextChar();
    while (currentChar != EOF)
    {
        stateTransition();
    }
}

char Tokenizer::getNextChar()
{
    currentPos = nextPos;
    if (nextChar == '\n')
    {
        nextPos.col = 0;
        nextPos.row++;
    }
    else nextPos.col++;
    currentChar = nextChar;
    // currentPos = nextPos;
    if (currentChar == EOF)
        return EOF;
    nextChar = fin.get();
    return currentChar;
}

void Tokenizer::stateTransition()
{
    switch (currentState)
    {
    case DFAState::INITIAL_STATE:
        analyseInitial();
        break;
    case DFAState::UINT_LITERAL_STATE:
        analyseUint();
        break;
    case DFAState::DOUBLE_LITERAL_STATE:
        analyseDouble();
        break;
    case DFAState::IDENT_STATE:
        analyseIdent();
        break;
    case DFAState::CHAR_LITERAL_STATE:
        analyseChar();
        break;
    case DFAState::STRING_LITERAL_STATE:
        analyseString();
        break;
    case DFAState::COMMENT_STATE:
        analyseComment();
        break;
    case DFAState::ERROR_STATE:
        throw Error(ErrorCode::ErrIncompleteExpression, currentPos);
    default:
        break;
    }
    // getNextChar();
}

void Tokenizer::analyseInitial()
{
    // 已经吃下了currentChar，现在的状态是Initial
    while(nextChar == ' ' || nextChar == '\n' || nextChar == '\r' || nextChar == '\t')
        getNextChar();
    auto pos = nextPos;
    // std::cout << pos << std::endl;
    if(nextChar == '+'){
        getNextChar();
        tokens.push_back(Token(TokenType::PLUS,pos,currentPos));
    }
    else if(nextChar == '-')
    {
        getNextChar();
        if(nextChar == '>')
        {
            getNextChar();
            tokens.push_back(Token(TokenType::ARROW,pos,currentPos));
        }
        else {
            tokens.push_back(Token(TokenType::MINUS,pos,currentPos));
        }
    }
    else if(nextChar == '*')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::MUL,pos,currentPos));
    }
    else if(nextChar == '/')
    {
        getNextChar();
        if(nextChar == '/')
        {
            getNextChar();
            currentState = DFAState::COMMENT_STATE;
        }
        else {
            tokens.push_back(Token(TokenType::DIV,pos,currentPos));
        }
    }
    else if(nextChar == '=')
    {
        getNextChar();
        if(nextChar == '=')
        {
            getNextChar();
            tokens.push_back(Token(TokenType::EQ,pos,currentPos));
        }
        else {
            tokens.push_back(Token(TokenType::ASSIGN,pos,currentPos));
        }
    }
    else if(nextChar == '!')
    {
        getNextChar();
        if(nextChar == '=')
        {
            tokens.push_back(Token(TokenType::NEQ,pos,currentPos));
        }
        else throw Error(ErrorCode::ErrAssignToConstant,currentPos);
    }
    else if(nextChar == '<')
    {
        getNextChar();
        if(nextChar == '=')
        {
            getNextChar();
            tokens.push_back(Token(TokenType::LE,pos,currentPos));
        }
        else {
            tokens.push_back(Token(TokenType::LT,pos,currentPos));
        }
    }
    else if(nextChar == '>')
    {
        getNextChar();
        if(nextChar == '=')
        {
            getNextChar();
            tokens.push_back(Token(TokenType::GE,pos,currentPos));
        }
        else {
            tokens.push_back(Token(TokenType::GT,pos,currentPos));
        }
    }
    else if(nextChar == '(')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::L_PAREN,pos,currentPos));
    }
    else if(nextChar == ')')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::R_PAREN,pos,currentPos));
    }
    else if(nextChar == '{')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::L_BRACE,pos,currentPos));
    }
    else if(nextChar == '}')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::R_BRACE,pos,currentPos));
    }
    else if(nextChar == ',')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::COMMA,pos,currentPos));
    }
    else if(nextChar == ':')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::COLON,pos,currentPos));
    }
    else if(nextChar == ';')
    {
        getNextChar();
        tokens.push_back(Token(TokenType::SEMICOLON,pos,currentPos));
    }
    else if(isdigit(nextChar))
    {
        // getNextChar();
        // 此时nextChar指向第一个数字
        currentState = DFAState::UINT_LITERAL_STATE;
    }
    else if(nextChar == '\'')
    {
        // getNextChar();
        // 此时nextChar指向'\''
        currentState = DFAState::CHAR_LITERAL_STATE;
    }
    else if(nextChar == '\"')
    {
        // getNextChar();
        // 此时nextChar指向"
        currentState = DFAState::STRING_LITERAL_STATE;
    }
    else if(nextChar == '_'||isalpha(nextChar))
    {
        // 此时nextChar指向标识符的第一个字符
        currentState = DFAState::IDENT_STATE;
    }
    else getNextChar();
}

void Tokenizer::analyseUint()
{
    // nextPos指向第一个数字的位置
    buffer.clear();
    long long value = 0;
    auto pos = nextPos;
    while (isdigit(nextChar))
    {
        buffer += nextChar;
        value = value*10 + nextChar - '0';
        getNextChar();
    }
    if(nextChar == '.')
    {
        buffer += nextChar;
        getNextChar();
        if(!isdigit(nextChar)) throw Error(ErrorCode::ErrAssignToConstant,currentPos);
        while(isdigit(nextChar)) buffer += nextChar,getNextChar();
        if(nextChar == 'e' || nextChar == 'E')
        {
            buffer += nextChar;
            getNextChar();
            if(nextChar == '+'||nextChar == '-')
            {
                buffer += nextChar;
                getNextChar();
            }
            if(!isdigit(nextChar)) throw Error(ErrorCode::ErrEOF,currentPos);
            while (isdigit(nextChar))
            {
                buffer += nextChar;
                getNextChar();
            }
        }
        tokens.push_back(Token(TokenType::DOUBLE_LITERAL,atof(buffer.c_str()),pos,currentPos));
    }
    else 
        tokens.push_back(Token(TokenType::UINT_LITERAL,value,pos,currentPos));
    buffer.clear();
    currentState = DFAState::INITIAL_STATE;
}
void Tokenizer::analyseDouble()
{
}
void Tokenizer::analyseString()
{
    auto pos = nextPos;
    getNextChar(); // 现在currentChar指向"
    buffer.clear();
    while(nextChar != '\"' && nextChar != EOF)
    {
        if(nextChar == '\\')
        {
            getNextChar();
            switch (nextChar)
            {
            case 'n':
                buffer += '\n';
                break;
            case 't':
                buffer += '\t';
                break;
            case 'r':
                buffer += '\r';
                break;
            case '\'':
                buffer += '\'';
                break;
            case '\"':
                buffer += '\"';
                break;
            case '\\':
                buffer += '\\';
                break;
            default:
                throw Error(ErrorCode::ErrAssignToConstant,currentPos);
                break;
            }
        }
        else buffer += nextChar;
        getNextChar();
    }
    if(nextChar == EOF) throw Error(ErrorCode::ErrAssignToConstant,currentPos);
    getNextChar();
    currentState = DFAState::INITIAL_STATE;
    tokens.push_back(Token(TokenType::STRING_LITERAL,buffer.c_str(),pos,currentPos));
}
void Tokenizer::analyseChar()
{
    getNextChar(); // 此时currentChar指向'\''
    char value;
    auto pos = nextPos;
    if(nextChar == '\\')
    {
        getNextChar();
        switch (nextChar)
            {
            case 'n':
                value = '\n';
                break;
            case 't':
                value = '\t';
                break;
            case 'r':
                value = '\r';
                break;
            case '\'':
                value = '\'';
                break;
            case '\"':
                value = '\"';
                break;
            case '\\':
                value = '\\';
                break;
            default:
                throw Error(ErrorCode::ErrAssignToConstant,currentPos);
                break;
            }
    }
    else {
        value = nextChar;
    }
    getNextChar();
    if(nextChar != '\'') throw Error(ErrorCode::ErrAssignToConstant,currentPos);
    getNextChar();
    currentState = DFAState::INITIAL_STATE;
    tokens.push_back(Token(TokenType::CHAR_LITERAL,value,pos,currentPos));
}
void Tokenizer::analyseComment()
{
    buffer.clear();
    auto pos = nextPos;
    while (nextChar != '\n' && nextChar != EOF)
    {
        buffer += nextChar;
        getNextChar();
    }
    tokens.push_back(Token(TokenType::COMMENT,buffer.c_str(),pos,currentPos));
    currentState = DFAState::INITIAL_STATE;
}
void Tokenizer::analyseIdent()
{
    buffer.clear();
    buffer += nextChar;
    auto pos = nextPos;
    getNextChar();
    while(nextChar == '_' || isdigit(nextChar)||isalpha(nextChar))
    {
        buffer += nextChar;
        getNextChar();
    }
    currentState = DFAState::INITIAL_STATE;
    // std::cout << "... "<< buffer << std::endl;
    if(buffer == "fn")
        tokens.push_back(Token(TokenType::FN_KW,pos,currentPos));
    else if(buffer == "let")
        tokens.push_back(Token(TokenType::LET_KW,pos,currentPos));
    else if(buffer == "const")
        tokens.push_back(Token(TokenType::CONST_KW,pos,currentPos));
    else if(buffer == "as")
        tokens.push_back(Token(TokenType::AS_KW,pos,currentPos));
    else if(buffer == "while")
        tokens.push_back(Token(TokenType::WHILE_KW,pos,currentPos));
    else if(buffer == "if")
        tokens.push_back(Token(TokenType::IF_KW,pos,currentPos));
    else if(buffer == "else")
        tokens.push_back(Token(TokenType::ELSE_KW,pos,currentPos));
    else if(buffer == "return")
        tokens.push_back(Token(TokenType::RETURN_KW,pos,currentPos));
    else if(buffer == "break")
        tokens.push_back(Token(TokenType::BREAK_KW,pos,currentPos));
    else if(buffer == "continue")
        tokens.push_back(Token(TokenType::CONTINUE_KW,pos,currentPos));
    else if(buffer == "int")
        tokens.push_back(Token(TokenType::TY,1ll,pos,currentPos));
    else if(buffer == "void")
        tokens.push_back(Token(TokenType::TY,0ll,pos,currentPos));
    else if(buffer == "double")
        tokens.push_back(Token(TokenType::TY,2ll,pos,currentPos));
    else tokens.push_back(Token(TokenType::IDENT,buffer.c_str(),pos,currentPos));
}