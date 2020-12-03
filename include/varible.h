#ifndef VARIBLE_H_
#define VARIBLE_H_
#include <string>

enum class Type
{
    INT,
    VOID,
    DOUBLE,
    CHAR
};
enum class Kind
{
    PARAM,
    FUNC,
    VAR
};
//////////////
/// \brief 变量
//////////////
class Varible
{
private :
    bool is_const;
    std::string name;

    std::string type; // 暂时使用一个string来表示类型
    int size;

    ///////////////////////////
    /// \brief 地址
    ///////////////////////////
    unsigned long long address;
public:
    Varible() = default;
    Varible(const std::string & type,const std::string & name,unsigned long long address,bool is_const = false) 
    {
        this->type = type;
        this->name = name;
        this->address = address;
        this->is_const = is_const;
        if( type == "u32") size = 4;
        else if(type == "double" || type == "f64" || type == "i64" || type == "u64" || type == "int") size = 8;
        else if(type == "u16") size = 2;
        else if(type == "u8") size = 1;
        else size = 0;
    }

    bool isConst() const { return is_const; }
    const std::string & getName() const { return name; }
    const std::string & getType() const { return type; }
    int getSize() const { return size; }
    unsigned long long getAddress() const { return address; }
};

#endif // VARIBLE_H_