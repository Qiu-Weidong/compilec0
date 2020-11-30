#ifndef POSITION_H_
#define POSITION_H_
#include <iostream>
struct position
{   
    //////////
    /// \brief 所在的行
    //////////
    int row;

    //////////
    /// \brief 所在的列
    //////////
    int col;

    ////////////////////////////
    /// \brief 构造函数
    /// \param row 行
    /// \param col 列
    ////////////////////////////
    position(int row=0,int col=0)
    {
        this->row = row;
        this->col = col;
    }
    friend std::ostream & operator<<(std::ostream & os, const position & p)
    {
        os << "(" << p.row << "," << p.col << ")";
        return os;
    }
};

#endif //POSITION_H_