#pragma once


#include <string>
#include <wtypes.h>
#include <stdint.h>

namespace mqwUtils
{
	//�������ַ�ת�����������ڷ��ص��ڴ���malloc����ģ�������Ҫ�ⲿfree
	std::wstring ANSIToUnicode(const char* strANSI);
	std::string UnicodeToANSI(const wchar_t* strUnicode);
	std::wstring UTF8ToUnicode(const char* strUTF8);
	std::string UnicodeToUTF8(const wchar_t* strUnicode);

	std::string ANSIToUTF8(const char* strANSI);

	std::string UTF8ToANSI(const char* strUTF8);


	/*���ַ�ת��Ϊ���ַ�ANSI - Unicode*/
	wchar_t* Acsi2WideByte(const char* mbs);
	/*���ַ�ת��Ϊ���ַ�Unicode - ANSI*/
	char* w2m(const wchar_t* wcs);
	std::wstring ANSI2Unicode(const std::string & str);
	std::string Unicode2ANSI(const std::wstring & wstr);
	std::wstring UTF82Unicode(const std::string & str);

	//�ַ��滻����
	void replace(LPSTR source, char look, char dest);
	
	// ���º����෴
	ULONG bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen); 
	// �ڴ����ݸ�ʽת��Ϊ�ַ��������ڴ�����0x33 0x61 0x41-> 3aA
	ULONG hex_to_bin(const char *src, ULONG len, char *out);       


	uint64_t bswap_64(uint64_t value);    // ����λת������ͬ
	ULONG bswap_32(ULONG value);
	USHORT bswap_16(ULONG value);
}
