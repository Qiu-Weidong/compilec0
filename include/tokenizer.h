#ifndef TOKENIZER_H_
#define TOKENIZER_H_
#include <fstream>
#include "position.h"
#include "token.h"
#include <vector>
#include <string>

///////////////////
/// \brief 有穷自动机的状态
///////////////////
enum class DFAState
{
    // 初始状态
    INITIAL_STATE,
    // 无符号整数状态
    UINT_LITERAL_STATE,
    // 浮点数状态
    DOUBLE_LITERAL_STATE,
    // 字符串字面值状态
    STRING_LITERAL_STATE,
    // 字符字面值状态
    CHAR_LITERAL_STATE,
    // 标识符状态
    IDENT_STATE,
    // 注释状态
    COMMENT_STATE,
    // 异常状态
    ERROR_STATE
};

class Tokenizer
{
private:
    ////////////////////
    /// \brief 当前位置
    ////////////////////
    Position currentPos;

    ////////////////////
    /// \brief 下一个位置
    ////////////////////
    Position nextPos;

    //////////////////
    /// \brief 读入的文件流
    //////////////////
    std::ifstream fin;

    //////////////////
    /// \brief 分析得到的token
    //////////////////
    std::vector<Token> tokens;

    //////////////////
    /// \brief 当前的字符
    //////////////////
    char currentChar;

    //////////////////
    /// \brief 上一个字符
    //////////////////
    // char previousChar;

    //////////////////
    /// \brief 下一个字符
    //////////////////
    char nextChar;

    /////////////////
    /// \brief 状态
    /////////////////
    DFAState currentState;

    //////////////////
    /// \brief 缓冲区
    //////////////////
    std::string buffer;

    /////////////////
    /// \brief 获取下一个字符
    /////////////////
    char getNextChar();

    //////////////////
    /// \brief 状态转移函数
    //////////////////
    void stateTransition();

    void analyseInitial();
    void analyseUint();
    void analyseDouble();
    void analyseString();
    void analyseChar();
    void analyseComment();
    void analyseIdent();
public:
    Tokenizer() = default;
    ~Tokenizer() = default;
    Tokenizer(const Tokenizer & ) = delete;
    Tokenizer(Tokenizer && ) = delete;
    Tokenizer & operator=(const Tokenizer &) = delete;
    Tokenizer & operator=(Tokenizer &&) = delete;
    void analyse(const char * file_name);
    const std::vector<Token> & getTokens() const { return tokens; }
};

#endif //TOKENIZER_H_

// // 加号状态
    // PLUS_STATE,
    // // 减号状态
    // MINUS_STATE,
    // // 除号状态
    // DIV_STATE,
    // // 乘号状态
    // MUL_STATE,
    // // 赋值状态
    // ASSIGN_STATE,
    // // 等号状态
    // EQ_STATE,
    // // 不等于状态
    // NEQ_STATE,
    // // 小于号状态
    // LT_STATE,
    // // 大于号状态
    // GT_STATE,
    // // 小于等于状态
    // LE_STATE,
    // // 大于等于状态
    // GE_STATE,
    // // 分号状态
    // SEMICOLON_STATE,
    // // 左括号状态
    // L_PAREN_STATE,
    // // 右括号状态
    // R_PAREN_STATE,