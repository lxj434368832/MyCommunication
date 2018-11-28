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

#ifndef LOGS
#define LOGS

#define MLOG(format, ...)  do{  \
	printf("FKLog msg: function:%s line:%d",__FUNCTION__,__LINE__);  \
	printf(format, ##__VA_ARGS__); printf("\n"); }while(0)
#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

#define Mcout std::cout<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"
#define Mcerr std::cerr<<"["<<__FUNCTION__<<":"<<__LINE__<<"]"

#endif // !LOGS

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
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::shutdown(x,2);::closesocket(x);x = INVALID_SOCKET;}}
#endif
