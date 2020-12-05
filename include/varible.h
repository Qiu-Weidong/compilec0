#ifndef VARIBLE_H_
#define VARIBLE_H_
#include <string>
#include <vector>
#include "position.h"
#include "output.h"

////////////////
/// \brief 变量具体的类型
////////////////
enum class Type
{
    // 整形
    INT,
    // void型
    VOID,
    // 浮点型
    DOUBLE,
    // 字符串
    STRING,
    BOOL
};
///////////////
/// \brief 符号表的条目类型，
///////////////
enum class Kind
{
    // 参数
    PARAM,
    // 函数，可以不要，将函数放在另一张表当中
    FUNC,
    // 变量
    VAR,
    // 全局变量
    GLOBAL
};

//////////////
/// \brief 变量
//////////////
class Varible
{
private :
    /////////////
    /// \brief 是否常量
    /////////////
    bool is_const;

    /////////////////
    /// \brief 变量名，如果是字符串常量，则保存字符串
    /////////////////
    std::string name;

    ///////////
    /// 参数还是普通变量
    ///////////
    Kind kind;

    //////////
    /// \brief 数据类型
    //////////
    Type type;

    //////////
    /// \brief 数据的size
    //////////
    int size;

    ///////////////////////////
    /// \brief 地址,如果是全部变量，则表示是第几个全局变量
    ///        如果是函数参数，则表示是该函数的第几个参数
    ///        如果是局部变量，则表示第几个局部变量
    ///        可能需要加入符号表中才分配
    ///////////////////////////
    unsigned int address;
public:
    Varible() = default;
    Varible(const Varible &) = default;
    Varible(Varible &&) = default;
    Varible & operator=(const Varible &) = default;
    Varible & operator=(Varible &&) = default;
    ~Varible() = default;

    Varible(const std::string & name,const Kind kind, const Type type,unsigned int address=0,bool is_const = false) 
    {
        this->type = type;
        this->name = name;
        this->address = address;
        this->is_const = is_const;
        this->kind = kind;
        if(type == Type::VOID) size = 0;
        else size = 8;
    }

    bool isConst() const { return is_const; }
    const std::string & getName() const { return name; }
    Type getType() const { return type; }
    Kind getKind() const { return kind; }
    int getSize() const { return size; }
    unsigned int getAddress() const { return address; }

    void setConst(bool is_const) { this->is_const = is_const;}
    void setAddress(unsigned int address) { this->address = address; }
    void setType(Type type) { this->type = type; }
    void setKind(Kind kind) { this->kind = kind; }
    void setSize(int size) { this->size = size; }
    void setName(const std::string & name) { this->name = name; }

    friend std::ostream & operator<<(std::ostream & os,const Varible & var);
};

//////////////////
/// \brief 变量的符号表
//////////////////
class VaribleTable
{
private:
    VaribleTable * parent;
    std::vector<Varible> table;
public:
    /////////////////////////////////////////////
    /// \brief 符号表的构造函数
    /// \param parent 父级符号表指针
    /////////////////////////////////////////////
    VaribleTable(VaribleTable * parent = nullptr) 
        : parent(parent) {}
    
    ////////////////////////////////////////////////////////
    /// \brief 符号表的构造函数
    /// \param parent 父级符号表引用
    ////////////////////////////////////////////////////////
    VaribleTable(VaribleTable & parent) 
        : parent(&parent) {}
    
    //////////////////////////////////////////////
    /// \brief 判断变量名在当前符号表种是否被定义
    /// \param var_name 要判断的变量名
    /// \return 是否被定义
    /// \note 仅考虑当前符号表，不考虑父级
    //////////////////////////////////////////////
    bool isDeclared(const std::string & var_name);

    //////////////////////////////////////////////////////
    /// \brief 将变量插入符号表
    /// \param var 要插入的变量
    /// \param pos 变量在程序中的位置，报错用
    /// \exception 如果已经定义，则抛出异常
    //////////////////////////////////////////////////////
    void insert(const Varible & var,const Position & pos);

    //////////////////////////////////////////////////////////////////////
    /// \brief 通过变量名在符号表中查找变量
    /// \param var_name 要查找的变量名
    /// \param pos 对变量的引用出现的位置，报错用
    /// \return 查找的变量的引用
    /// \exception 如果当前表以及所有的父级表中都没有，抛出异常
    /// \note 如果当前表中没有，则继续查找父级表
    //////////////////////////////////////////////////////////////////////
    const Varible & get(const std::string & var_name,const Position & pos);

    ////////////////////////////////////////////////////////
    /// \brief 判断当前符号表是否是全局符号表
    /// \return 是否是全局符号表
    ////////////////////////////////////////////////////////
    bool isGlobalTable() const { return parent == nullptr; }

    //////////////////////////
    /// \brief 获取全局符号表
    /// \return 全局符号表的引用
    //////////////////////////
    VaribleTable & getGlobal() 
    { 
        if(parent == nullptr) return *this;
        return parent->getGlobal();
    }

    //////////////////////////////////
    /// \brief 清空符号表
    //////////////////////////////////
    void clear() { table.clear(); }

    ///////////////////////////////////////////////////////////////////////////
    /// \brief 友元函数，用于输出
    /// \param os 用于输出的流
    /// \param vr 待输出的符号表
    /// \return 输出流的引用
    ///////////////////////////////////////////////////////////////////////////
    friend std::ostream & operator<<(std::ostream & os,const VaribleTable & vr);
};
#endif // VARIBLE_H_