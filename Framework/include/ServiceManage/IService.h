#pragma once
/************************************************************************
* function��ʹ�÷���ĳ���ӿ�
* author :	������
* datetime��2018-04-12
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include <tchar.h>
#include <windows.h>

__interface IService
{
	void Start(DWORD argc, TCHAR* argv[]);
	void Stop();
};
