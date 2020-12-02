#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

////////////////////
/// \brief 虚拟机的指令
///        压栈和弹栈以8字节为单位
///        寻址单位为一个字节
///    
///        数据类型有u64、i64、f64
////////////////////
enum class Operation
{
    NOP, // 空指令
    PUSH, // 需要一个操作数u64，将操作数压栈。
    POP, // 将一个slot弹栈
    POPN, // 需要一个u32操作数，弹栈n个slot
    DUP, // 复制栈顶slot，并将其压栈
    LOCA,// 需要一个u32参数，加载off个slot处局部变量的地址
    ARGA,// 需要一个参数u32，加载off个slot处参数/返回值的地址
    GLOBA,// 需要一个参数u32，加载第n个全局变量/常量的地址
    LOAD_8,// 弹栈一个地址，从这个地址加载8位压栈
    LOAD_16,// 同上
    LOAD_32,// 
    LOAD_64, //
    STORE_8, // 弹栈一个地址，一个值，将值的前八位存入地址
    STORE_16, //
    STORE_32, //
    STORE_64, //
    ALLOC, // 弹栈一个size，压栈一个地址，表示在堆上分配size个字节的内存
    FREE, // 弹栈一个地址，表示释放掉内存
    STACKALLOC, // 需要一个参数u32，在当前栈顶分配size个slot，并初始化为0
    ADD_I, // 弹栈两个参数，相加，将结果压栈
    SUB_I, // 同上
    MUL_I, // 同上
    DIV_I, // 同上
    ADD_F, // 
    SUB_F, //
    MUL_F, //
    DIV_F, //
    DIV_U, //
    SHL, // 弹栈两个slot，将结果栈顶 << 次栈顶压栈
    SHR, // 同上
    AND, // 
    OR, // 
    XOR, // 
    NOT, // 弹栈一个slot，并将!栈顶压栈
    CMP_I, // 弹栈两个slot，并将比较结果压栈
    CMP_U, // 
    CMP_F, // 
    NEG_I, // 弹栈一个slot，将其取反并压栈
    NEG_F, //  
    ITOF, // 弹栈一个slot，并将其转换为浮点数
    FTOI, // 弹栈一个slot，并将其转换为整数
    SHRL, // 同SHR，但这里式逻辑右移
    SET_LT, //  弹栈lhs，如果lhs < 0 ，则推入1，否则推入0
    SET_GT, //  弹栈lhs，若lhs > 0,则推入1，否则推入0
    BR, // 需要参数i32， 无条件跳转偏移off
    BR_FALSE, // 需要一个参数i32，弹栈一个test，若test为0则跳转off
    BR_TRUE, //  同上，test为1跳转
    CALL, // 需要一个参数u32，调用该编号的函数
    RET, // 从当前函数返回
    CALLNAME, // 需要一个参数u32，调用名称与编号为id的全局变量内容相同的函数
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

class Instruction
{
private:
    /////////////
    /// \brief 操作
    /////////////
    Operation op;

    /////////////
    /// \brief 操作数
    /////////////

};
#endif // INSTRUCTION_H_