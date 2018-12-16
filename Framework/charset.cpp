#include "charset.h"
#include <vector>
#include <codecvt>



//*****************************************************************************
//	����˵����	64bit��λ��λ
//	����ԭ�ͣ�	DWORD64 bswap_64(DWORD64 value)
//  ������      value��Ҫ��λ��ֵ
//  ����ֵ��    ���ػ�λ֮���ֵ
//  �÷���		
//*****************************************************************************
uint64_t charset::bswap_64(uint64_t value)
{
	return (((uint64_t)bswap_32(value)) << 32) | bswap_32((value >> 32));;
}

//*****************************************************************************
//	����˵����	32bit��λ
//	����ԭ�ͣ�	ͬ��
//  ������      ͬ��
//  ����ֵ��    ͬ��
//  �÷���		ͬ��
//*****************************************************************************
ULONG charset::bswap_32(ULONG value)
{
	_asm mov eax, value
	_asm bswap eax
}

//*****************************************************************************
//	����˵����	16bit��λ
//	����ԭ�ͣ�	ͬ��
//  ������      ͬ��
//  ����ֵ��    ͬ��
//  �÷���		ͬ��
//*****************************************************************************
USHORT charset::bswap_16(ULONG value)
{
	_asm mov eax, value
	_asm and eax, 0xFFFF
	_asm xchg ah, al
}

//*****************************************************************************
//	����˵����	�ڴ��е��ַ���ת��Ϊ����ı���ʽ�����ڴ��ַ�������3aA-> 33 61 41
//	����ԭ�ͣ�	ULONG bin_to_hex(char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
//  ������      hlenÿ����ʾ������ktext�ָ���
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
ULONG charset::bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
{
	char *hout = out;
	char *ctext = "0123456789ABCDEF";
	if (mode)
		ctext = "0123456789abcdef";
	for (ULONG i = 0; i< len; i++)
	{
		*out = ctext[(BYTE)(src[i]) >> 4];
		out++;
		*out = ctext[src[i] & 0x0F];
		out++;
		if ((i % hlen) == (hlen - 1))
		{
			*(WORD *)(out) = 0x0A0D;
			out += 2;
		}
		else if (ktext && *ktext)
		{
			*out = *ktext;
			out++;
		}
	}
	*out = '\0';
	return out - hout;
}

//*****************************************************************************
//	����˵����	�ڴ������ı���ʽת��Ϊ�ڴ��ַ������ݣ����ڴ����ݵ��ı���ʽ33 61 41 -> 3aA
//	����ԭ�ͣ�	ULONG hex_to_bin(char *src, ULONG len, char *out)
//  ������      
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
ULONG charset::hex_to_bin(const char *src, ULONG len, char *out)
{
	BYTE num, bit, bits = 0;
	ULONG i = 0, ti = 0;
	while (true)
	{
		num = src[ti];
		ti++;
		if (num >= 0x30 && num <= 0x39)
			num -= 0x30;
		else if (num >= 0x41 && num <= 0x46)
			num -= 0x37;
		else if (num >= 0x61 && num <= 0x66)
			num -= 0x57;
		else if ((!len && !num) || (len && ti >= len))
			break;
		else
			continue;

		if (bits)
		{
			bit |= num;
			out[i] = bit;
			i++;
		}
		else
			bit = num << 4;
		bits ^= 1;
	}
	return i;
}



//*****************************************************************************
//	����˵����	ANSIת��ΪUNICODE�ַ���
//	����ԭ�ͣ�	int ans_to_uni(LPSTR src_str, LPWSTR dst_str, int len)
//  ������      src_str ��ת����ANSI�ַ�����dst_strת��֮���UNICODE�ַ�����len����
//  ����ֵ��    ����ת��֮��ĳ���
//  �÷���		
//*****************************************************************************
int charset::ans_to_uni(const char* src_str, wchar_t* dst_str, int len)
{
	if (!len) len = strlen(src_str) + 1;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src_str, -1, dst_str, len);
	return len = lstrlenW(dst_str) * 2;
}

//*****************************************************************************
//	����˵����	ͬ���෴
//	����ԭ�ͣ�	
//  ������      
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
int charset::uni_to_ans(const wchar_t* src_str, char* dst_str, int len)
{
	if (!len) len = lstrlenW(src_str);
	len = WideCharToMultiByte(936, 0, src_str, len, dst_str, len * 2, 0, 0);
	dst_str[len] = 0;
	return len;
}


//*****************************************************************************
//	����˵����	UTF-8�ַ���ת��ΪANSI�ַ���
//	����ԭ�ͣ�	int utf8_to_ans(LPSTR src_str,LPSTR dst_str,int len)
//  ������      
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
int charset::utf8_to_ans(const char* src_str, char* dst_str, int len)
{
	int	tlen;
	LPWSTR *tstr;
	if (!len)	len = lstrlenA(src_str);
	tstr = (LPWSTR *)malloc(len * 2);
	tlen = MultiByteToWideChar(CP_UTF8, 0, src_str, len, (LPWSTR)tstr, len * 2);
	len = WideCharToMultiByte(936, 0, (LPCWSTR)tstr, tlen, (LPSTR)dst_str, len * 2, 0, 0);
	free(tstr);
	dst_str[len] = 0;
	return len;
}

//UTF-8
//*****************************************************************************
//	����˵����	
//	����ԭ�ͣ�	
//  ������      
//  ����ֵ��    
//  �÷���		
//*****************************************************************************
int charset::ans_to_utf8(const char* src_str, char* dst_str, int len)
{
	int	tlen;
	LPWSTR *tstr;
	if (!len)	len = lstrlenA(src_str);
	tstr = (LPWSTR *)malloc(len * 2);
	tlen = MultiByteToWideChar(936, 0, src_str, len, (LPWSTR)tstr, len * 2);
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)tstr, tlen, (LPSTR)dst_str, len * 2, 0, 0);
	free(tstr);
	dst_str[len] = 0;
	return len;
}

// ANSIתUTF-8(c++11)        ���ʹ��s_to_ws()���ws_to_utf8()
const std::string charset::ans_to_utf8(std::string const &a_str)
{
	std::vector<wchar_t> buff(a_str.size());
#ifdef _MSC_VER
	std::locale loc("");//"zh-CN"
#else
	std::locale loc("zh_CN.GB18030");
#endif
	wchar_t* ws_next = nullptr;
	const char* sz_next = nullptr;
	mbstate_t state = {};
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).in(state,
			a_str.data(), a_str.data() + a_str.size(), sz_next,
			buff.data(), buff.data() + buff.size(), ws_next);

	if (std::codecvt_base::ok == res)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
		return cutf8.to_bytes(std::wstring(buff.data(), ws_next));
	}

	return "";
}

// UTF-8תANSI(c++11)
const std::string charset::utf8_to_ans(const std::string &u_str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
	std::wstring wTemp = cutf8.from_bytes(u_str);
#ifdef _MSC_VER
	std::locale loc("zh-CN");
#else
	std::locale loc("zh_CN.GB18030");
#endif
	const wchar_t* ws_next = nullptr;
	char* sz_next = nullptr;
	mbstate_t state = {};

	std::vector<char> buff(wTemp.size() * 2);
	int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
		(loc).out(state,
			wTemp.data(), wTemp.data() + wTemp.size(), ws_next,
			buff.data(), buff.data() + buff.size(), sz_next);

	if (std::codecvt_base::ok == res)
	{
		return std::string(buff.data(), sz_next);
	}
	return "";
}

const std::string charset::ws_to_s(const std::wstring& src)
{
	std::locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (src.size() + 1) * wchar_size);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t out_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		delete[] data_to;
		return std::string("");
	}
}

const std::wstring charset::s_to_ws(const std::string& src)
{
	std::locale sys_locale("");

	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, src.size() + 1);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t in_state = { 0 };
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		delete[] data_to;
		return std::wstring(L"");
	}
}

const std::string charset::ws_to_utf8(const std::wstring& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(src);
}

const std::wstring charset::utf8_to_ws(const std::string& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(src);
}

