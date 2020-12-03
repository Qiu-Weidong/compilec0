#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <string>
#include <vector>
#include "varible.h"
#include "instruction.h"
#include "output.h"
#include "error.h"

//////////////////
/// \brief 函数的声明
//////////////////
class FunctionDecl
{
private:
    /////////
    /// \brief 给函数分配一个fid
    /////////
    int fid;

    //////////////
    /// \brief 返回类型
    //////////////
    Type retType;

    ////////////////
    /// \brief 函数名
    ////////////////
    std::string name;

    ///////////////////////////
    /// \brief 参数列表，如果有返回值，那么第一个参数的address为返回值的偏移
    ///        没有返回值，那么第一个参数的address为0，以此类推
    ///////////////////////////
    std::vector<Varible> params;
    
public:
    FunctionDecl() = default;
    FunctionDecl(const FunctionDecl &) = default;
    FunctionDecl(FunctionDecl &&) = default;
    FunctionDecl & operator=(const FunctionDecl &) = default;
    FunctionDecl & operator=(FunctionDecl &&) = default;
    ~FunctionDecl() = default;

    FunctionDecl(const std::string & name,Type ret_type) : name(name),retType(ret_type) { params.clear(); }
    void addParam(const Varible & vb) ;
};

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

    //////////////////////
    /// \brief 符号表，每个函数都有自己独立的符号表
    //////////////////////
    VaribleTable varibles;

    /////////////////
    /// \brief 函数名称
    /////////////////
    std::string name;
public:
    Function(const std::string & name, int rslots=0,int pslots=0,int lslots=0, VaribleTable * parent=nullptr) 
        : name(name), return_slots(rslots),param_slots(pslots),loca_slots(lslots),varibles(parent) { fid = -1; }
    ~Function() = default;

    // 全删了
    Function(const Function &) = delete;
    Function(Function &&) = delete;
    Function & operator=(const Function & ) = delete;
    Function & operator=(Function &&) = delete;

    
    const VaribleTable & getVaribleTable() const { return varibles; }
    const std::vector<Instruction> & getInstructions() const { return instructions; }
    void addInstruction(const Instruction & instruction) { instructions.push_back(instruction); }
    int getReturnSlots() const { return return_slots; }
    int getParamSlots() const { return param_slots; }
    int getLocaSlots() const { return loca_slots; }
    int getFid() const { return fid; }
    const std::string & getName() const { return name; }
    void setReturnSlots(int rslots) { this->return_slots = rslots; }
    void setParamSlots(int pslots) { this->param_slots = pslots; }
    void setLocaSlots(int lslots) { this->loca_slots = lslots; } 
    void setFid(int fid) { this->fid = fid; }
    
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
    // 全删了
    FunctionTable(const FunctionTable &) = delete;
    FunctionTable(FunctionTable &&) = delete;
    FunctionTable & operator=(const FunctionTable &) = delete;
    FunctionTable & operator=(FunctionTable && ) = delete;

    bool isDeclared(const std::string & name);
    void insert(Function & fn, const Position & pos);
    Function & get(const std::string & name,const Position & pos);

    friend std::ostream & operator<<(std::ostream & os,const FunctionTable & ft);
};
#endif // FUNCTION_H_