#ifndef OUTPUT_H_
#define OUTPUT_H_
#include <fstream>

///////////////////
/// \brief 检查机器是大端序还是小端序
///////////////////
bool isBigEndian();

///////////////////////////////////////////////
/// \brief 将p之后的size字节的内容按大端序用二进制输出
///////////////////////////////////////////////
void write(std::ostream & os,void * p,int size);

#endif // OUTPUT_H_