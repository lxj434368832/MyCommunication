#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>

namespace mqwUtils
{
	//�������ַ�ת�����������ڷ��ص��ڴ���malloc����ģ�������Ҫ�ⲿfree
	wchar_t * ANSIToUnicode(const char* str);
	char * UnicodeToANSI(const wchar_t* str);
	wchar_t * UTF8ToUnicode(const char* str); 
	char * UnicodeToUTF8(const wchar_t* str);

	/*���ַ�ת��Ϊ���ַ�Unicode - ANSI*/
	char* w2m(const wchar_t* wcs);
	/*���ַ�ת��Ϊ���ַ�ANSI - Unicode*/
	wchar_t* m2w(const char* mbs);

	char* ANSIToUTF8(const char* str);
	void  ANSIToUTF8(std::string& str);

	char* UTF8ToANSI(const char* str);
	void UTF8ToANSI(std::string& str);

	//�ַ��滻����
	void replace(LPSTR source, char look, char dest);
}
