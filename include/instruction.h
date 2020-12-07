#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_
#include <iostream>
#include <fstream>
#include "output.h"

////////////////////
/// \brief 虚拟机的指令
///        压栈和弹栈以8字节为单位
///        寻址单位为一个字节
///
///        数据类型有u64、i64、f64
///        将类型设定为unsigned char是为了方便输出
///        将值设置为虚拟机指令编码
////////////////////
enum class Operation : unsigned char
{
    NOP = 0x00,        // 空指令
    PUSH = 0x01,       // 需要一个操作数u64，将操作数压栈。
    POP = 0x02,        // 将一个slot弹栈
    POPN = 0x03,       // 需要一个u32操作数，弹栈n个slot
    DUP = 0x04,        // 复制栈顶slot，并将其压栈
    LOCA = 0x0a,       // 需要一个u32参数，加载off个slot处局部变量的地址
    ARGA = 0x0b,       // 需要一个参数u32，加载off个slot处参数/返回值的地址
    GLOBA = 0x0c,      // 需要一个参数u32，加载第n个全局变量/常量的地址
    LOAD_8 = 0x10,     // 弹栈一个地址，从这个地址加载8位压栈
    LOAD_16 = 0x11,    // 同上
    LOAD_32 = 0x12,    //
    LOAD_64 = 0x13,    //
    STORE_8 = 0x14,    // 弹栈一个地址，一个值，将值的前八位存入地址
    STORE_16 = 0x15,   //
    STORE_32 = 0x16,   //
    STORE_64 = 0x17,   //
    ALLOC = 0x18,      // 弹栈一个size，压栈一个地址，表示在堆上分配size个字节的内存
    FREE = 0x19,       // 弹栈一个地址，表示释放掉内存
    STACKALLOC = 0x1a, // 需要一个参数u32，在当前栈顶分配size个slot，并初始化为0
    ADD_I = 0x20,      // 弹栈两个参数，相加，将结果压栈
    SUB_I = 0x21,      // 同上
    MUL_I = 0x22,      // 同上
    DIV_I = 0x23,      // 同上
    ADD_F = 0x24,      //
    SUB_F = 0x25,      //
    MUL_F = 0x26,      //
    DIV_F = 0x27,      //
    DIV_U = 0x28,      //
    SHL = 0x29,        // 弹栈两个slot，将结果栈顶 << 次栈顶压栈
    SHR = 0x2a,        // 同上
    AND = 0x2b,        //
    OR = 0x2c,         //
    XOR = 0x2d,        //
    NOT = 0x2e,        // 弹栈一个slot，并将!栈顶压栈
    CMP_I = 0x30,      // 弹栈两个slot，并将比较结果压栈
    CMP_U = 0x31,      //
    CMP_F = 0x32,      //
    NEG_I = 0x34,      // 弹栈一个slot，将其取反并压栈
    NEG_F = 0x35,      //
    ITOF = 0x36,       // 弹栈一个slot，并将其转换为浮点数
    FTOI = 0x37,       // 弹栈一个slot，并将其转换为整数
    SHRL = 0x38,       // 同SHR，但这里式逻辑右移
    SET_LT = 0x39,     //  弹栈lhs，如果lhs < 0 ，则推入1，否则推入0
    SET_GT = 0x3a,     //  弹栈lhs，若lhs > 0,则推入1，否则推入0
    BR = 0x41,         // 需要参数i32， 无条件跳转偏移off
    BR_FALSE = 0x42,   // 需要一个参数i32，弹栈一个test，若test为0则跳转off
    BR_TRUE = 0x43,    //  同上，test为1跳转
    CALL = 0x48,       // 需要一个参数u32，调用该编号的函数
    RET = 0x49,        // 从当前函数返回
    CALLNAME = 0x4a,   // 需要一个参数u32，调用名称与编号为id的全局变量内容相同的函数
    SCAN_I = 0x50,     // 从标准输入读入一个整数n，入栈
    SCAN_C = 0x51,     // 从标准输入读入一个字符c，入栈
    SCAN_F = 0x52,     // 从标准输入读入一个浮点数f，入栈。
    PRINT_I = 0x54,    // 弹栈一个有符号整数写到标准输出。
    PRINT_C = 0x55,    // 弹栈一个字符写到标准输出
    PRINT_F = 0x56,    // 弹栈一个浮点数f，向标准输出写入f
    PRINT_S = 0x57,    // 从栈中弹出一个i，表示字符串。
    PRINTLN = 0x58,    // 向标准输出写入一个换行
    PANIC = 0xfe       // 恐慌/强行退出
};

class Instruction
{
private:
    /////////////
    /// \brief 操作
    /////////////
    Operation op;

    /////////////
    /// \brief 操作数所占的字节数
    ///        size的取值可能是1、2、4、8
    /////////////
    int size;

    /////////////////
    /// \brief 操作数,只有4种类型
    /////////////////
    union
    {
        unsigned int u32;
        unsigned long long u64;
        int i32;
        long long i64;
        double f64;
    } num;

public:
    ////////////////////////////
    /// \brief 默认的6个构造/析构函数
    ////////////////////////////
    Instruction() = default;
    Instruction(const Instruction &) = default;
    Instruction(Instruction &&) = default;
    Instruction &operator=(const Instruction &) = default;
    Instruction &operator=(Instruction &&) = default;
    ~Instruction() = default;

    //////////////////////////
    /// \brief 没有操作数的指令的构造函数
    //////////////////////////
    Instruction(Operation op) : op(op), size(0) {}
    Instruction(Operation op, unsigned int u32) : op(op), size(4) { this->num.u32 = u32; }
    Instruction(Operation op, unsigned long long u64) : op(op), size(8) { this->num.u64 = u64; }
    Instruction(Operation op, int i32) : op(op), size(4) { this->num.i32 = i32; }
    Instruction(Operation op, long long i64) : op(op), size(8) { this->num.i64; }
    Instruction(Operation op, double f64) : op(op), size(8) { this->num.f64 = f64; }

    void setNum(unsigned int u32)
    {
        size = 4;
        this->num.u32 = u32;
    }

    void setNum(int i32)
    {
        size = 4;
        this->num.i32 = i32;
    }

    void setNum(unsigned long long u64)
    {
        size = 8;
        this->num.u64 = u64;
    }

    void setNum(long long i64)
    {
        size = 8;
        this->num.i64 = i64;
    }

    void setNum(double f64)
    {
        size = 8;
        this->num.f64 = f64;
    }

#ifndef DEBUG
    ///////////////////////////////////////////////////////////////////////////
    /// \brief 用于输出的友元函数
    ///////////////////////////////////////////////////////////////////////////
    friend std::ostream &operator<<(std::ostream &os, const Instruction &i)
    {
        write(os, (void *)(&i.op), sizeof(i.op));
        write(os, (void *)(&i.num), i.size);
        return os;
    }
#else
    friend std::ostream &operator<<(std::ostream &os, const Instruction &i)
    {
        switch (i.op)
        {
            case Operation::NOP:
                os << "nop   " << std::endl;
                break;
            case Operation::PUSH:
                os << "push  " << i.num.u64 << std::endl;
                break;
            case Operation::POP:
                os << "pop   " << std::endl;
                break;
            case Operation::POPN:
                os << "popn  " << std::endl;
                break;
            case Operation::DUP:
                os << "dup   " << std::endl;
                break;
            case Operation::LOCA:
                os << "loca  " << i.num.u32 << std::endl;
                break;
            case Operation::GLOBA:
                os << "globa " << i.num.u32 << std::endl;
                break;
            case Operation::ARGA:
                os << "arga  " << i.num.u32 << std::endl;
                break;
            case Operation::LOAD_64:
                os << "load.64"  << std::endl;
                break;
            case Operation::STORE_64:
                os << "store.64"  << std::endl;
                break;
            case Operation::ALLOC:
                os << "alloc   " << std::endl;
                break;
            case Operation::FREE:
                os << "free    " << std::endl;
                break;
            case Operation::STACKALLOC:
                os << "stackalloc  "<< i.num.u32 << std::endl;
                break;
            case Operation::ADD_I:
                os << "add.i    " << std::endl;
                break;
            case Operation::ADD_F:
                os << "add.f    " << std::endl;
                break;
            case Operation::SUB_F:
                os << "sub.f    " << std::endl;
                break;
            case Operation::SUB_I:
                os << "sub.i    " << std::endl;
                break;
            case Operation::MUL_I:
                os << "mul.i    " << std::endl;
                break;
            case Operation::MUL_F:
                os << "mul.f    " << std::endl;
                break;
            case Operation::DIV_I:
                os << "div.i    " << std::endl;
                break;
            case Operation::DIV_F:
                os << "div.f    " << std::endl;
                break;
            case Operation::DIV_U:
                os << "div.u    " << std::endl;
                break;
            case Operation::SHL:
                os << "shl    " << std::endl;
                break;
            case Operation::SHR:
                os << "shr    " << std::endl;
                break;
            case Operation::SHRL:
                os << "shrl   " << std::endl;
                break;
            case Operation::AND:
                os << "and    " << std::endl;
                break;
            case Operation::OR:
                os << "or    " << std::endl;
                break;
            case Operation::NOT:
                os << "not    " << std::endl;
                break;
            case Operation::XOR:
                os << "xor    " << std::endl;
                break;
            case Operation::CMP_I:
                os << "cmp.i    " << std::endl;
                break;
            case Operation::CMP_F:
                os << "cmp.f    " << std::endl;
                break;
            case Operation::CMP_U:
                os << "cmp.u    " << std::endl;
                break;
            case Operation::NEG_I:
                os << "neg.i    " << std::endl;
                break;
            case Operation::NEG_F:
                os << "neg.f    " << std::endl;
                break;
            case Operation::ITOF:
                os << "itof    " << std::endl;
                break;
            case Operation::FTOI:
                os << "ftoi    " << std::endl;
                break;
            case Operation::SET_GT:
                os << "set.gt    " << std::endl;
                break;
            case Operation::SET_LT:
                os << "set.lt    " << std::endl;
                break;
            case Operation::BR:
                os << "br    " << i.num.i32 << std::endl;
                break;
            case Operation::BR_FALSE:
                os << "br.false  "<<i.num.i32 << std::endl;
                break;
            case Operation::BR_TRUE:
                os << "br.true   " <<i.num.i32<< std::endl;
                break;
            case Operation::RET:
                os << "ret    " << std::endl;
                break;
            case Operation::CALL:
                os << "call    " << i.num.u32 << std::endl;
                break;
            case Operation::CALLNAME:
                os << "callname    "<<i.num.u32 << std::endl;
                break;
            case Operation::SCAN_C:
                os << "scan.c    " << std::endl;
                break;
            case Operation::SCAN_F:
                os << "scan.f    " << std::endl;
                break;
            case Operation::SCAN_I:
                os << "scan.i    " << std::endl;
                break;
            case Operation::PRINT_C:
                os << "print.c    " << std::endl;
                break;
            case Operation::PRINT_F:
                os << "print.f    " << std::endl;
                break;
            case Operation::PRINT_I:
                os << "print.i    " << std::endl;
                break;
            case Operation::PRINT_S:
                os << "print.s    " << std::endl;
                break;
            case Operation::PRINTLN:
                os << "println    " << std::endl;
                break;
            case Operation::PANIC:
                os << "panic    " << std::endl;
                break;
        }
        return os;
    }
#endif //DEBUG
};

#endif // INSTRUCTION_H_