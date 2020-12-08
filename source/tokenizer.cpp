#include "tokenizer.h"
#include "error.h"
#include <cctype>
#include <cstdlib>
void Tokenizer::analyse(const char *file_name)
{
    fin.open(file_name);
    if (!fin.is_open())
        throw Error(ErrorCode::NoInput,"Input not ready!");
    
    currentChar = nextChar = '\0';
    buffer.clear();
    currentState = DFAState::INITIAL_STATE;
    currentPos = Position(-1,-1);
    nextPos = Position(0,-1);
    tokens.push_back(Token(TokenType::NONE,currentPos,currentPos));
    getNextChar();
    while (currentChar != EOF)
    {
        stateTransition();
    }
    // 添加一个哨兵token
    tokens.push_back(Token(TokenType::NONE,currentPos,currentPos));
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
        throw Error(ErrorCode::UnknownErr,"unknown error!",currentPos);
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
            getNextChar();
            tokens.push_back(Token(TokenType::NEQ,pos,currentPos));
        }
        else throw Error(ErrorCode::InvalidSymbol,"\'=\' can\'t follow \'!\'!",currentPos);
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
    auto pos = nextPos;
    while (isdigit(nextChar))
    {
        buffer += nextChar;
        getNextChar();
    }
    if(nextChar == '.')
    {
        buffer += nextChar;
        getNextChar();
        if(!isdigit(nextChar)) throw Error(ErrorCode::InvalidDoubleLiteral,"\'.\' must be followed with a digit!",currentPos);
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
            if(!isdigit(nextChar)) throw Error(ErrorCode::InvalidDoubleLiteral,"invalid double literal!",currentPos);
            while (isdigit(nextChar))
            {
                buffer += nextChar;
                getNextChar();
            }
        }
        tokens.push_back(Token(TokenType::DOUBLE_LITERAL,buffer,pos,currentPos));
    }
    else 
        tokens.push_back(Token(TokenType::UINT_LITERAL,buffer,pos,currentPos));
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
                throw Error(ErrorCode::UnknownChar,"invalid escape sequence!",currentPos);
                break;
            }
        }
        else buffer += nextChar;
        getNextChar();
    }
    if(nextChar == EOF) throw Error(ErrorCode::InvalidStringLiteral,"invalid string literal!",currentPos);
    getNextChar();
    currentState = DFAState::INITIAL_STATE;
    tokens.push_back(Token(TokenType::STRING_LITERAL,buffer,pos,currentPos));
}
void Tokenizer::analyseChar()
{
    getNextChar(); // 此时currentChar指向'\''
    buffer.clear();
    auto pos = nextPos;
    if(nextChar == '\\')
    {
        getNextChar();
        switch (nextChar)
            {
            case 'n':
                buffer = "\n";
                break;
            case 't':
                buffer = "\t";
                break;
            case 'r':
                buffer = "\r";
                break;
            case '\'':
                buffer = "\'";
                break;
            case '\"':
                buffer = "\"";
                break;
            case '\\':
                buffer = "\\";
                break;
            default:
                throw Error(ErrorCode::UnknownChar,"invalid escape sequence!",currentPos);
                break;
            }
    }
    else {
        buffer = "" + nextChar;
    }
    getNextChar();
    if(nextChar != '\'') throw Error(ErrorCode::InvalidCharLiteral,"invalid char literal!",currentPos);
    getNextChar();
    currentState = DFAState::INITIAL_STATE;
    tokens.push_back(Token(TokenType::CHAR_LITERAL,buffer,pos,currentPos));
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
    // 注释不添加到tokens列表当中。
    // tokens.push_back(Token(TokenType::COMMENT,buffer,pos,currentPos));
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
        tokens.push_back(Token(TokenType::TY,buffer,pos,currentPos));
    else if(buffer == "void")
        tokens.push_back(Token(TokenType::TY,buffer,pos,currentPos));
    else if(buffer == "double")
        tokens.push_back(Token(TokenType::TY,buffer,pos,currentPos));
    else tokens.push_back(Token(TokenType::IDENT,buffer,pos,currentPos));
}