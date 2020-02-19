//**********************************************************************
//*		˵�����˹�����������ļ�����socket��ʼ�����ٽ�������ʼ����
//*			  ���ӡ��ַ���ת����ĳЩϵͳ���������İ�װ������IAT��
//*			  TCP/UDP���Ӻ�����
//*		���ߣ�PotalXu��CheLi
//*		���ڣ�2012.01.01���޸�������2016.01.11 by PotalXu��
//**********************************************************************
#pragma once

#include <windows.h>
#include <string>

namespace mqwUtils
{
//***********ϵͳ��װ����**************//
#ifdef WIN32
    std::string GetModulePath(HMODULE module, const char *f_name);        // ��ȡ�ļ�·����
	HMODULE GetCurrentModuleHandle();                                       // ��õ�ǰģ��������dllģ��
	bool IsModuleNameMatch(HMODULE module, char *pName);                         // �ж�ģ����
	int GetProcessorCount();					// ��ȡPC��CPU����
	//guid����:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string MakeGuid();

	std::string GetErrorMessage(DWORD dw);

	bool GetMacByGetAdaptersInfo(std::string& macOUT);
	bool GetMacByGetAdaptersAddresses(std::string& macOUT);
#endif

}
