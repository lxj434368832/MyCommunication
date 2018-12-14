// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>


// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "../Framework/zxl_logging.h"

#ifndef LOGS
#define LOGS
#define MLOG(format, ...)  do{  \
	printf("MLog msg: function:%s line:%d",__FUNCTION__,__LINE__);  \
	printf(format, ##__VA_ARGS__); printf("\n"); }while(0)

#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

#define Mcout std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#define Mcerr std::cerr<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#endif // !LOGS

#ifndef ZX_LOGGING_H_
static HANDLE consolehwnd = ::GetStdHandle(STD_OUTPUT_HANDLE);
#define logm() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[O]:"
#define loge() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[E]:"
#define logd() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[D]:"
#define logt() ::SetConsoleTextAttribute(consolehwnd, 4);std::cout<<"[T]:"<<::GetTickCount64()
//#define logt() ::system("color 4");std::cout<<"[T]:"
#else
#define logm() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<message>]"<<std::endl
#define loge() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<error>]"<<std::endl
#define logd() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<debug>]"<<std::endl
#define logt() LogInfo()<<"["<<__FUNCTION__<<":"<<__LINE__<<"|<trace>]"<<std::endl
#endif

// �ͷ�ָ���
#ifndef RELEASE
#define RELEASE(x) {if(x!=NULL){delete x;x=NULL;}}
#endif

// �ͷž����
#ifndef RELEASE_HANDLE
#define RELEASE_HANDLE(x) {if(x != NULL && x != INVALID_HANDLE_VALUE){::CloseHandle(x);x = NULL;}}
#endif

// �ͷ�Socket��
#ifndef RELEASE_SOCKET
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::closesocket(x);x = INVALID_SOCKET;}}
#endif
