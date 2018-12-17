#pragma once

#include <string>
#include <windows.h>

class charset
{
public:
	charset() {};
	~charset() {};
public:
	static ULONG bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen);     // ���º����෴
	static ULONG hex_to_bin(const char *src, ULONG len, char *out);                                        // �ڴ����ݸ�ʽת��Ϊ�ַ��������ڴ�����0x33 0x61 0x41-> 3aA
	static int uni_to_ans(const wchar_t *src_str, char *dst_str, int len);                                  // UNICODEתANSI
	static int ans_to_uni(const char *src_str, wchar_t *dst_str, int len);                                  // ANSIתUNICODE
	static int utf8_to_ans(const char *src_str, char *dst_str, int len);                                    // UTF-8תANSI
	static int ans_to_utf8(const char *src_str, char *dst_str, int len);                                    // ANSIתUTF-8

	static const std::string ans_to_utf8(const std::string &a_str);                                         // ANSIתUTF-8(c++11)
	static const std::string utf8_to_ans(const std::string &u_str);                                         // UTF-8תANSI(c++11)
	static const std::string ws_to_s(const std::wstring& src);
	static const std::wstring s_to_ws(const std::string& src);
	static const std::string ws_to_utf8(const std::wstring& src);
	static const std::wstring utf8_to_ws(const std::string& src);

	static uint64_t bswap_64(uint64_t value);                                                        // ����λת������ͬ
	static ULONG bswap_32(ULONG value);
	static USHORT bswap_16(ULONG value);
};
