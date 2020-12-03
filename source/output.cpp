#include "output.h"

bool isBigEndian()
{
    int u = 0x1;
    char * v = (char *)(&u);
    // 如果是大端序，则保存为0x00000001,否则0x01000000
    return *v == 0;
}

void write(std::ostream & os,void * p,int size)
{
    if(isBigEndian()){
        os.write((char *)p,size);
    }
    else {
        for(int i=size-1;i>=0;i--)
            os.write((char *)p+i,sizeof(char));
    }
}