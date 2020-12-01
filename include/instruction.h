#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

////////////////////
/// \brief 虚拟机的指令
////////////////////
enum class Operation
{
    NOP, // 空指令
    PUSH, // 
    POP, //
    POPN, //
    DUP, //
    LOCA,// 
    ARGA,//
    GLOBA,//
    LOAD_8,//
    LOAD_16,//
    LOAD_32,//
    LOAD_64, //
    STORE_8, //
    STORE_16, //
    STORE_32, //
    STORE_64, //
    ALLOC, //
    FREE, //
    STACKALLOC, //
    ADD_I, //
    SUB_I, //
    MUL_I, //
    DIV_I, //
    ADD_F, //
    SUB_F, //
    MUL_F, //
    DIV_F, //
    DIV_U, //
    SHL, // 
    SHR, // 
    AND, //
    OR, // 
    XOR, // 
    NOT, // 
    CMP_I, // 
    CMP_U, // 
    CMP_F, // 
    NEG_I, //
    NEG_F, // 
    ITOF, // 
    FTOI, // 
    SHRL, // 
    SET_LT, // 
    SET_GT, // 
    BR, // 
    BR_FALSE, // 
    BR_TRUE, // 
    CALL, // 
    RET, // 
    CALLNAME, // 
    SCAN_I, // 从标准输入读入一个整数n，入栈
    SCAN_C, // 从标准输入读入一个字符c，入栈
    SCAN_F, // 从标准输入读入一个浮点数f，入栈。
    PRINT_I, // 弹栈一个有符号整数写到标准输出。
    PRINT_C, // 弹栈一个字符写到标准输出
    PRINT_F, // 弹栈一个浮点数f，向标准输出写入f
    PRINT_S, // 从栈中弹出一个i，表示字符串。
    PRINTLN, // 向标准输出写入一个换行
    PANIC // 恐慌/强行退出
};
#endif // INSTRUCTION_H_