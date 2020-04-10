// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#pragma once

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "..\Framework\include\LogFile.h"	

#ifndef Log
static HANDLE consolehwnd = ::GetStdHandle(STD_OUTPUT_HANDLE);
#define logm() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[M]:"
#define loge() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[E]:"
#define logd() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[D]:"
#define logt() ::SetConsoleTextAttribute(consolehwnd, 4);std::cout<<"[T]:"<<::GetTickCount64()
//#define logt() ::system("color 4");std::cout<<"[T]:"
#endif

// �ͷ�ָ���
#ifndef RELEASE
#define RELEASE(x) {if(x != nullptr){delete x; x=nullptr;}}
#endif

// �ͷž����
#ifndef RELEASE_HANDLE
#define RELEASE_HANDLE(x) {if(x != NULL && x != INVALID_HANDLE_VALUE){::CloseHandle(x);x = NULL;}}
#endif

// �ͷ�Socket��
#ifndef RELEASE_SOCKET
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::closesocket(x);x = INVALID_SOCKET;}}
#endif


#define DECLARE_DATA(Class) /
inline Class##Data* d_func() { return static_cast<Class##Data *>(d); } /
inline const Class##Data* d_func() const { return static_cast<const Class##Data *>(d); }

#define M_D(Class) Class##Data * const d = d_func()
