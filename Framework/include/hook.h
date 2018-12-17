#pragma once

#include <windows.h>

class hook
{
private:
	FARPROC			api_proc;
	unsigned char	hook_hex[8];    //������
	unsigned char	src_hex[8];     //Դ����

public:
	hook() = default;
	~hook() = default;

public:
	//HOOK api
	void hook_api(HINSTANCE dll_inst, char *api_name, FARPROC call_proc);
	//HOOK code
	void hook_code(FARPROC proc, FARPROC call_proc);
	void set();		//��
	void revert();	//��ԭ
	static int replace_iat(HANDLE process, const char *dll_name, const char *api_name, DWORD &call_code, DWORD hook_code);	//IAT Hook
};
