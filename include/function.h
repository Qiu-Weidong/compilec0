#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <string>
#include <vector>
#include "varible.h"
#include "instruction.h"
#include "output.h"
#include "error.h"

//////////////
/// \brief 一个表示函数的类
//////////////
class Function
{
private:
    /////////////////
    /// \brief 为函数添加一个id，这个id要在插入到符号表
    ///        的时候才分配
    /////////////////
    unsigned int fid;

    /////////////////////////
    /// \brief 返回值所占的槽数
    /////////////////////////
    unsigned int return_slots;

    ///////////////////////////
    /// \brief 参数的槽数
    ///////////////////////////
    unsigned int param_slots;

    ///////////////////////
    /// \brief 局部变量的槽数
    ///////////////////////
    unsigned int loca_slots; 

    /////////////////////////////////////
    /// \brief 指令
    /////////////////////////////////////
    std::vector<Instruction> instructions;

    /////////////////
    /// \brief 函数名称
    /////////////////
    std::string name;

    ///////////////////
    /// \brief 返回值类型
    ///////////////////
    Type return_type;

    ///////////////////////////////
    /// \brief 参数类型列表
    ///////////////////////////////
    std::vector<Type> param_types;

public:
    Function() = default;
    Function(const std::string & name,Type return_type, int rslots=0,int pslots=0,int lslots=0) 
        : name(name), return_type(return_type), return_slots(rslots),param_slots(pslots),loca_slots(lslots) { fid = -1; }
    ~Function() = default;

    Function(const Function &) = default;
    Function(Function &&) = default;
    Function & operator=(const Function & ) = default;
    Function & operator=(Function &&) = default;

    // get
    const std::vector<Instruction> & getInstructions() const { return instructions; }
    int getReturnSlots() const { return return_slots; }
    int getParamSlots() const { return param_slots; }
    int getLocaSlots() const { return loca_slots; }
    int getFid() const { return fid; }
    int nextLoca() { return loca_slots++; }
    const std::string & getName() const { return name; }
    Type getReturnType() const { return return_type; }
    const std::vector<Type> & getParamTypes() const { return param_types; }

    // set
    void setName(const std::string & name) { this->name = name; }
    void setReturnSlots(int rslots) { this->return_slots = rslots; }
    void setParamSlots(int pslots) { this->param_slots = pslots; }
    void setLocaSlots(int lslots) { this->loca_slots = lslots; } 
    void setFid(int fid) { this->fid = fid; }
    void setReturnType(Type type) { this->return_type = type; }
    void addInstruction(const Instruction & instruction) { instructions.push_back(instruction); }
    void addParamType(Type type) { this->param_types.push_back(type),this->param_slots++; }
    // output
    friend std::ostream & operator<<(std::ostream & os,const Function & f);
};

class FunctionTable
{
private:
    int next_fid; // 下一个分配的fid
    std::vector<Function> functions;
public:
    FunctionTable() { next_fid = 0; }
    ~FunctionTable() = default;
    FunctionTable(const FunctionTable &) = default;
    FunctionTable(FunctionTable &&) = default;
    FunctionTable & operator=(const FunctionTable &) = default;
    FunctionTable & operator=(FunctionTable && ) = default;

    /////////////////////////////////////////
    /// \brief 判断名称为name的函数是否已经定义
    /// \param name 函数名
    /// \return 是否定义
    /////////////////////////////////////////
    bool isDeclared(const std::string & name);

    ///////////////////////////////////////////////
    /// \brief 向函数表中插入函数
    /// \param fn 要插入的函数
    /// \param pos 函数定义的位置
    /// \return 为该函数分配的fid
    /// \exception 如果已经定义，抛出异常
    ///////////////////////////////////////////////
    int insert(Function & fn, const Position & pos);

    /////////////////////////////////////////////////////////////
    /// \brief 按函数名获取函数
    /// \param name 函数名
    /// \param pos 函数引用的位置
    /// \return 函数的引用
    /// \exception 如果函数没用定义，抛出异常
    /////////////////////////////////////////////////////////////
    Function & get(const std::string & name,const Position & pos);

    ////////////////////////////////////////////
    /// \brief 按函数fid获取函数
    /// \param fid 函数fid
    /// \param pos 函数引用出现的位置
    /// \return 获取到的函数的引用
    /// \exception 如果没有找到，抛出异常
    ////////////////////////////////////////////
    Function & get(int fid,const Position & pos);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief 输出函数
    /// \param os 用于输出的流
    /// \param ft 要输出的函数
    /// \return 输出流
    ////////////////////////////////////////////////////////////////////////////
    friend std::ostream & operator<<(std::ostream & os,const FunctionTable & ft);
};
#endif // FUNCTION_H_