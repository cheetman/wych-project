#ifndef TEST_H
#define TEST_H

#include <Windows.h>
//#include <WinUser.h>
unsigned  __stdcall testThread3(void * pParam)
{
    while(1){
        Sleep(10000);
    }
    return 0;
}
#endif // TEST_H
