#pragma once

// TODO:  在此处引用程序需要的其他头文件

#include "..\3rdParty\Framework\include\LogFile.h"	//此头文件包含了windows.h

#ifndef LOGS
#define LOGS
#define MLOG(format, ...)  do{  \
	printf("[%s:%d]",__FUNCTION__,__LINE__);  \
	printf(format, ##__VA_ARGS__); printf("\n"); }while(0)

#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

//#define Mcout std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
//#define Mcerr std::cerr<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#endif // !LOGS

/*
#ifndef ZX_LOGGING_H_
static HANDLE consolehwnd = ::GetStdHandle(STD_OUTPUT_HANDLE);
#define logm() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[O]:"
#define loge() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[E]:"
#define logd() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[D]:"
#define logt() ::SetConsoleTextAttribute(consolehwnd, 4);std::cout<<"[T]:"<<::GetTickCount64()
//#define logt() ::system("color 4");std::cout<<"[T]:"
#endif*/

// 释放指针宏
#ifndef RELEASE
#define RELEASE(x) {if(x != nullptr){delete x; x=nullptr;}}
#endif

// 释放句柄宏
#ifndef RELEASE_HANDLE
#define RELEASE_HANDLE(x) {if(x != NULL && x != INVALID_HANDLE_VALUE){::CloseHandle(x);x = NULL;}}
#endif

// 释放Socket宏
#ifndef RELEASE_SOCKET
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::closesocket(x);x = INVALID_SOCKET;}}
#endif
