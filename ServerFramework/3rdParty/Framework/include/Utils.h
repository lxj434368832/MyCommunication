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
    std::string get_module_path(HMODULE module, const char *f_name);        // ��ȡ�ļ�·����
	HMODULE get_self_module_handle();                                       // ��õ�ǰģ��������dllģ��
	bool is_process_a(HMODULE module, char *pname);                         // �ж�ģ����
	int get_num_of_processors();					// ��ȡPC��CPU����
	//guid����:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string make_guid();

	std::string GetErrorMessage(DWORD dw);

	bool GetMacByGetAdaptersInfo(std::string& macOUT);
	bool GetMacByGetAdaptersAddresses(std::string& macOUT);

#endif
    bool is_time(time_t ms, time_t dms);           // �Ƿ����ʱ��
    bool is_time_v(time_t &ms, time_t dms);        // ����ʱ���޸�

}
