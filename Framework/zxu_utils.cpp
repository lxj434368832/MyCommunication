//**********************************************************************
//*
//*	说明：	此头文件包含了文件处理、socket初始化、临界区锁初始化、
//*			钩子、字符集转换、某些系统公共函数的包装函数、IAT、
//*			TCP/UDP连接函数等
//*	作者：	PotalXu、CheLi
//*	日期：	2012.01.01（修改整合于2016.01.11 by PotalXu）
//*
//**********************************************************************
// 此处的两个头文件必须在最前面，否则会报错
#ifdef WIN32
#include <atlstr.h>
#include <mswsock.h>
#endif

#include "zxu_utils.h"
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <dbghelp.h>
#include <cstring>
#include <locale>
#include <codecvt>
#include <fstream>
#include <cctype>

#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE (1)
#endif
#pragma warning(disable : 4996)
#endif

using namespace zxu;
#ifdef WIN32
//*****************************************************************************
//	函数说明：	获得模块路径
//	函数原型：	void get_module_path(HMODULE module, char *f_name, char *out_f_n)
//  参数：      module模块句柄、f_name文件名、out_f_n包括路径在内的全名
//  返回值：    无
//  用法：		如果module为NULL，获取的文件名为(自身模块路径+f_name)
//*****************************************************************************
std::string zxu::get_module_path(HMODULE module, const char *f_name)
{
	char cpath[MAX_PATH] = {0};
	char cdrive[MAX_PATH] = {0};
	char cdir[MAX_PATH] = {0};
    GetModuleFileNameA(module, cpath, MAX_PATH);
    _splitpath(cpath, cdrive, cdir, 0, 0);
    std::stringstream tmp_str;
    tmp_str << cdrive << cdir << f_name;
    return tmp_str.str();
}


//*****************************************************************************
//	函数说明：	获得当前模块句柄包括dll模块
//	函数原型：	HMODULE get_self_module_handle()
//  参数：      无
//  返回值：    当前模块句柄
//  用法：		
//*****************************************************************************
HMODULE zxu::get_self_module_handle()
{
	MEMORY_BASIC_INFORMATION mbi;
	return ((VirtualQuery(get_self_module_handle, &mbi, sizeof(mbi)) != 0) ? (HMODULE) mbi.AllocationBase : NULL);
}


//*****************************************************************************
//	函数说明：	判断模块名
//	函数原型：	bool is_process_a(HMODULE Module, char *pname)
//  参数：      module需要判断的模块句柄、pname需要判断的模块名
//  返回值：    如果模块句柄与模块名正确返回true，否则返回false
//  用法：		
//*****************************************************************************
bool zxu::is_process_a(HMODULE module, char *pname)
{
	char cPath[MAX_PATH] = {0};
	char cFile[MAX_PATH] = {0};
	GetModuleFileNameA(module, cPath, MAX_PATH);
	_splitpath(cPath, 0, 0, cFile, 0);
	if (!stricmp(cFile, pname)) 
		return true;
	return false;
}


//*****************************************************************************
//	函数说明：	获得处理器的数量
//	函数原型：	int get_num_of_processors()
//  参数：      无
//  返回值：    数量
//  用法：		
//*****************************************************************************
int zxu::get_num_of_processors()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}
std::string zxu::make_guid()
{
	char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		sprintf_s(buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
	return std::string(buf);
}
#endif

//*****************************************************************************
//	函数说明：	根据s获取socket名
//	函数原型：	int get_sock_name(SOCKET s, sockaddr_in *sock)
//  参数：      s查询的socket句柄、sock获取的数据
//  返回值：    len
//  用法：		
//*****************************************************************************
int zxu::sockets::get_sock_name(sock_fd s, sockaddr_in *sock)
{
	sockaddr sockdata;
	int len = sizeof(sockdata);
	len = getsockname(s, &sockdata, &len);
	memcpy(sock, &sockdata.sa_data, sizeof(sockdata.sa_data));
	return len;
}


//*****************************************************************************
//	函数说明：	TCP连接初始化
//	函数原型：	SOCKET tcp_link(char * ip, int port)
//  参数：      ip地址、port端口
//  返回值：    成功返回socket句柄值，否则返回0
//  用法：		
//*****************************************************************************
sock_fd zxu::sockets::tcp_link(const std::string &ip, int port)
{
	struct	sockaddr_in	sock;
	struct	hostent	*host = 0;

    sock_fd hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	if (connect(hsocket, (struct sockaddr *)&sock, sizeof(sock)) != SOCKET_ERROR) 
        return hsocket;
	closesocket(hsocket); 
    return 0;
}

//*****************************************************************************
//	函数说明：	TCP监听初始化
//	函数原型：	SOCKET tcp_listen(u_short port)
//  参数：      port端口
//  返回值：    正确hsocket、 错误0
//  用法：		同上
//*****************************************************************************
sock_fd zxu::sockets::tcp_listen(const std::string &ip, u_short port)
{
    sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket) 
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
	if(!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// 绑定地址和端口
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock))) 
	{
		closesocket(hsocket);
		return 0;
	}
	// 开始进行监听
	if (SOCKET_ERROR == listen(hsocket,SOMAXCONN))
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}

//*****************************************************************************
//	函数说明：	UDP连接初始化
//	函数原型：	SOCKET udp_link(char * ip, int port, sockaddr_in &sock)
//  参数：      ip地址、port端口、sock为SOCKADDR_IN结构
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
sock_fd zxu::sockets::udp_link(const std::string &ip, int port, sockaddr_in &sock)
{
	struct hostent *host = 0;

    sock_fd hsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	return hsocket;
}

//*****************************************************************************
//	函数说明：	UDP监听初始化
//	函数原型：	SOCKET udp_listen(u_short port)
//  参数：      port端口
//  返回值：    正确hsocket、 错误0
//  用法：		同上
//*****************************************************************************
sock_fd zxu::sockets::udp_listen(const std::string &ip, u_short port)
{
    sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket) 
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
    if (!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// 绑定地址和端口
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock))) 
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}


//*****************************************************************************
//	函数说明：	IP地址转换，由DWORD转为网络字符串类型
//	函数原型：	const char* inet_ntoa(unsigned long ip_addr)
//  参数：      （ip_addr）DWORD类型的IP地址
//  返回值：    正确IP地址、 错误XXX.XXX.XXX.XXX
//  用法：		
//*****************************************************************************
const char* zxu::sockets::inet_ntoa(unsigned long ip_addr)
{
	static char* lpErrorAddr = "XXX.XXX.XXX.XXX";

	//使用系统函数转换
	char*	lpAddress = ::inet_ntoa(*(in_addr*)(&ip_addr));
	if(NULL == lpAddress)
	{
		return lpErrorAddr;
	}
	return lpAddress;
}

//*****************************************************************************
//	函数说明：	根据length接收数据
//	函数原型：	int recv_to_end(sock_fd s,char *buffer,int length,int flag)
//  参数：      如recv一致
//  返回值：    如果接收完length个数据，返回length，否则错误
//  用法：		和recv一致
//*****************************************************************************
int zxu::sockets::recv_to_end(sock_fd s, char *buffer, int length, int flag)
{
    char* ptr = buffer;
    int byteLeft = length;
    int l = 0;
    do{
        l = recv(s, ptr, byteLeft, flag);
        if (l == 0)
        {
            break;
        }
        ptr += l;
        byteLeft -= l;
    } while (byteLeft > 0 && l > 0);
    return length - byteLeft;
}

//*****************************************************************************
//	函数说明：	根据length发送数据
//	函数原型：	int send_to_end(sock_fd s,char *buffer,int length,int flag)
//  参数：      如recv一致
//  返回值：    如果发送完length个数据，返回length，否则错误
//  用法：		和上方法一致
//*****************************************************************************
int zxu::sockets::send_to_end(sock_fd s, char *buffer, int length, int flag)
{	// 总共需要发送的，已经发送的长度
    int total_bytes = length, bytes_sent = 0;
    do
    {
        int send_bytes = total_bytes - bytes_sent;
        int ret_bytes = send(s, buffer + bytes_sent, send_bytes, flag);
        if (ret_bytes < 0)
            break;
        bytes_sent += ret_bytes;
    } while (bytes_sent != total_bytes);
    return bytes_sent;
}

//*****************************************************************************
//	函数说明：	获取文件大小
//	函数原型：	long get_file_size(FILE *fp)
//  参数：      文件句柄fp
//  返回值：    返回文件大小
//  用法：		获取某个文件的大小
//*****************************************************************************
long zxu::files::get_file_size()
{ 
	long curpos = 0, length = 0; 
    curpos = ftell(m_fp);
    fseek(m_fp, 0L, SEEK_END);
    length = ftell(m_fp);
    fseek(m_fp, curpos, SEEK_SET);
	return length; 
}


//*****************************************************************************
//	函数说明：	读取文件
//	函数原型：	long file_read_string(char *filename, CString &out)
//  参数：      file_name文件名、out_buf文件内容输出buf
//  返回值：    返回文件大小
//  用法：		
//*****************************************************************************
long zxu::files::file_read_string(char *file_name, std::string &out_buf)
{
	FILE *fp = fopen(file_name,"rb");
	if (!fp) 
        return 0;
	long len = get_file_size();
    out_buf.resize(len);
	fread((void *)out_buf.data(), len, 1, fp);
	fclose(fp);
	return len;
}


//*****************************************************************************
//	函数说明：	读取文件
//	函数原型：	char *zxu::FileRead(char *file_name, char *fbuf, int &len)
//  参数：      文件名file_name、输出内存fbuf、输出长度len)
//  返回值：    返回输出内存
//  用法：		如果参数fbuf为NULL，则分配内存，则需要在外部用完后释放内存
//*****************************************************************************
char *zxu::files::file_read(char *file_name, char *fbuf, int &len)
{
	FILE *fp = fopen(file_name,"rb");
	if (!fp) return 0;
	if (!len) len = get_file_size();
	if (!fbuf) fbuf = (char *)malloc(len + 1);
	fread(fbuf, len, 1, fp);
	fbuf[len] = 0;
	fclose(fp);
	return fbuf;
}


//*****************************************************************************
//	函数说明：	保存文件
//	函数原型：	bool file_save(char *filename, char *fbuf, int len)
//  参数：      file_name保存文件名、fbuf内存段、len长度
//  返回值：    正确返回true、否则返回false
//  用法：		
//*****************************************************************************
bool zxu::files::file_save(char *file_name, char *fbuf, int len)
{
	FILE *fp = fopen(file_name,"ab+");
	if (!fp) return false;
	fwrite(fbuf, 1, len, fp);
	fclose(fp);
	return true;
}


//*****************************************************************************
//	函数说明：	判断是否大于时间
//	函数原型：	bool is_time(ULONG &ms, ULONG dms)
//  参数：      ms前一个记录时间、dms时间间隔
//  返回值：    如果当前时间比之前记录时间大于时间间隔，返回true，否则返回false
//  用法：		用来判断是否超时
//*****************************************************************************
bool zxu::is_time(time_t ms, time_t dms)
{
    time_t sms = time(0);
	return (sms - ms) > dms;
}


//*****************************************************************************
//	函数说明：	判断是否大于时间，如果大于则修改
//	函数原型：	bool is_time_v(ULONG &ms, ULONG dms)
//  参数：      同上
//  返回值：    大于则修改返回true，否则返回false
//  用法：		判断是否超时，如果超时就修改记录时间为当前时间
//*****************************************************************************
bool zxu::is_time_v(time_t &ms, time_t dms)
{
    time_t sms = time(0);
	if((sms - ms) > dms)
    {
        ms = sms; 
        return true;
    }
	return false;
}

void hook::hook_api(HINSTANCE dll_inst, char *api_name, FARPROC call_proc)
{
    api_proc = GetProcAddress(dll_inst, api_name);
    hook_code(api_proc, call_proc);
}

//HOOK code
void hook::hook_code(FARPROC proc, FARPROC call_proc)
{
    DWORD old_protect(0);
    api_proc = proc;
    BYTE * buff = &hook_hex[0];
    *(BYTE *)(buff) = 0xE9;
    buff++;
    *(DWORD *)(buff) = (DWORD)call_proc - (DWORD)api_proc - 5;
    VirtualProtect(api_proc, 8, PAGE_EXECUTE_READWRITE, &old_protect);
    memcpy(src_hex, api_proc, 5);
    memcpy(api_proc, hook_hex, 5);
}

void hook::set()		//钩
{
    memcpy(api_proc, hook_hex, 5);
}

void hook::revert()	//还原
{
    memcpy(api_proc, src_hex, 5);
}

//*****************************************************************************
//	函数说明：	IAT Hook
//	函数原型：	int replace_iat(xxx)
//  参数：      hProcess进程句柄、pDllName为Dll名、pApiName需要修复IAT函数名、CallCode修改地址、HookCode调用地址
//  返回值：    0
//  用法：		
//*****************************************************************************
int zxu::hook::replace_iat(HANDLE hProcess, const char *pDllName, const char *pApiName, DWORD &CallCode, DWORD HookCode)
{
	//HANDLE hProcess = GetModuleHandle(0);
	DWORD dwSize = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImageImport = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hProcess, true, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwSize);
	if (!pImageImport) return 1;
	PIMAGE_IMPORT_BY_NAME pImageImportByName = 0;
	PIMAGE_THUNK_DATA pImageThunkOriginal = 0;
	PIMAGE_THUNK_DATA pImageThunkReal  = 0;

	while (true)
	{
		if (!pImageImport->Name) return 2;
		else if (!strcmpi((char*)((PBYTE)hProcess + pImageImport->Name), pDllName)) break;
		++pImageImport;
	}

	pImageThunkOriginal = (PIMAGE_THUNK_DATA)((PBYTE)hProcess + pImageImport->OriginalFirstThunk);
	pImageThunkReal = (PIMAGE_THUNK_DATA)((PBYTE)hProcess + pImageImport->FirstThunk);
	while (pImageThunkOriginal->u1.Function)
	{
		if ((pImageThunkOriginal->u1.Ordinal & IMAGE_ORDINAL_FLAG) != IMAGE_ORDINAL_FLAG)
		{
			pImageImportByName = (PIMAGE_IMPORT_BY_NAME)((PBYTE)hProcess + pImageThunkOriginal->u1.AddressOfData);
			if (!strcmpi(pApiName, (char*)pImageImportByName->Name))
			{
				MEMORY_BASIC_INFORMATION mbi_thunk;
				VirtualQuery(pImageThunkReal, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
				VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect);
				if (HookCode)
				{
					CallCode = pImageThunkReal->u1.Function;
					pImageThunkReal->u1.Function = HookCode;
				}else{
					pImageThunkReal->u1.Function = CallCode;
				}
				DWORD dwOldProtect;
				VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect);
				break;
			}
		}
		++pImageThunkOriginal;
		++pImageThunkReal;
	}
	return 0;
}

//*****************************************************************************
//	函数说明：	生成数据 LOG  按小时分文件记录
//	函数原型：	bool save_mem_log(char * f_name,char * sstr,char * src,int len)
//  参数：      f_name全路径文件名、sstr记录说明、src需要记录数据的指针、len长度
//  返回值：    true
//  用法：		
//*****************************************************************************
bool zxu::files::save_mem_log(char * f_name, char * sstr, char * src, int len)
{
    char cdrive[MAX_PATH] = { 0 };
    char cdir[MAX_PATH] = { 0 };
    char cfile[MAX_PATH] = { 0 };
    _splitpath(f_name, cdrive, cdir, cfile, 0);
    char file_name[MAX_PATH] = { 0 };
    time_t t = time(0);
    tm tmp_time_struct;
    localtime_s(&tmp_time_struct, &t);
    strftime(file_name, sizeof(file_name), cfile, &tmp_time_struct);		//"%Y-%m-%d %H:%M:%S.log"
    std::string full_file_name("");
    full_file_name.append(cdrive);
    full_file_name.append(cdir);
    full_file_name.append(file_name);

    FILE *fp = NULL;
    fopen_s(&fp, full_file_name.c_str(), "ab+");
    if (!fp)
        return false;
    if (sstr)
        fputs(sstr, fp);
    if (src && len)
    {
        char *tbuff = (char *)malloc(len * 5);
        charset::bin_to_hex(src, len, tbuff, 0, " ", 0x10);
        fputs(tbuff, fp);
        fputs("\r\n", fp);
        free(tbuff);
    }
    fclose(fp);
    return true;
}


//*****************************************************************************
//	函数说明：	生成文本LOG  按小时分文件记录
//	函数原型：	bool save_log(char * name,char * ctext,...)
//  参数：      f_name文件名、ctext参数队列
//  返回值：    true
//  用法：		
//*****************************************************************************
bool zxu::files::save_log(char * f_name, char * ctext, ...)
{
    va_list argptr;
    char filename[MAX_PATH];
    va_start(argptr, ctext);
    time_t t = time(0);
    tm tmp_time_struct;
    localtime_s(&tmp_time_struct, &t);
    strftime(filename, sizeof(filename), f_name, &tmp_time_struct);
    FILE *fp = NULL;
    fopen_s(&fp, filename, "ab+");
    if (!fp)
        return false;

    vfprintf(fp, ctext, argptr);
    fclose(fp);
    va_end(argptr);
    return true;
}

//*****************************************************************************
//	函数说明：	文件操作类的带参数的构造函数
//	函数原型：	
//  参数：      file_name文件名，mode为操作文件的格式，name_type表示了日志文件名称的命名方式(非0则文件名为时间xx)
//  返回值：    无
//  用法：		带参数的创建对象			
//*****************************************************************************
zxu::files::files(const char * file_name, const char * mode, int name_type)
{
	m_file_length = 0;
	m_file_buffer = NULL;
	memset(m_mode,0,sizeof(m_mode));
	memset(m_file_name,0,MAX_PATH);
	memset(m_printf_buf,0,MAX_LOG_BUF_LEN);
	m_buf_len = 0;
	memcpy_s(m_mode,sizeof(m_mode),mode,sizeof(mode));
	if(name_type)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		char t_file_name[MAX_PATH] = {0};
		char temp_path[MAX_PATH] = {0};
		sprintf(t_file_name,"(%d-%d-%d)%s",time.wYear,time.wMonth,time.wDay,file_name);
		GetModuleFileNameA(NULL,temp_path,MAX_PATH);
		(strrchr(temp_path, '\\'))[1]=0;
		strcat_s(temp_path,MAX_PATH,"Log");				// Log日志文件夹
		DWORD resoult = GetFileAttributesA(temp_path);	// 检测是否存在，如果不存在该文件夹便建立
		if (resoult == -1)
			CreateDirectoryA(temp_path,NULL);
		char temp_file_name[MAX_PATH] = {0};
		sprintf(temp_file_name,"%s\\%s",temp_path,t_file_name);
		memcpy_s(m_file_name,MAX_PATH,temp_file_name,strlen(temp_file_name));
		m_fp = fopen(temp_file_name,m_mode);
	}
	else
	{
		memcpy_s(m_file_name,MAX_PATH,file_name,strlen(file_name));
		m_fp = fopen(file_name,m_mode);
	}
	if(m_fp)
		read_file_buff_len();
}

//*****************************************************************************
//	函数说明：	file_handler类的析构函数
//	函数原型：	~file_handler()
//  参数：      无
//  返回值：    无
//  用法：		
//*****************************************************************************
zxu::files::~files()
{
	if(m_buf_len)
		fprintf(m_fp,m_printf_buf);
	if(m_fp)
		fclose(m_fp);
	if(m_file_buffer)
		delete m_file_buffer;
	m_file_buffer = NULL;
}

//*****************************************************************************
//	函数说明：	打印输出到文件
//	函数原型：	void dbg_print_to_file(char * ctext,...)
//  参数：      ctext参数列表
//  返回值：    无
//  用法：		
//*****************************************************************************
void zxu::files::dbg_print_to_file(char * ctext, ...)
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	va_list arg_list;
	va_start(arg_list,ctext);
	vfprintf(m_fp,ctext,arg_list);
	va_end(arg_list);
}

//*****************************************************************************
//	函数说明：	获取当前文件缓冲区的指针地址
//	函数原型：	char * get_file_buff()
//  参数：      无
//  返回值：    返回文件缓冲地址
//  用法：		内部调用
//*****************************************************************************
char * zxu::files::get_file_buff()
{
    std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	read_file_buff_len();
	return m_file_buffer;
}

//*****************************************************************************
//	函数说明：	获取文件大小和数据
//	函数原型：	void read_file_buff_len()
//  参数：      无
//  返回值：    无
//  用法：		用于开始打开文件时
//*****************************************************************************
void zxu::files::read_file_buff_len()
{
    std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if(!m_fp)
		return;
	m_file_length = get_file_size();
	if(strncmp(m_mode,"rb",2) != 0)
	{
		fclose(m_fp);
		m_fp = fopen(m_file_name,"rb");
		if (m_fp)
		{
			if(m_file_buffer)
				delete m_file_buffer;
			m_file_buffer = new char[m_file_length + 1];
			fread(m_file_buffer,m_file_length,1,m_fp);
			m_file_buffer[m_file_length] = 0;
			fclose(m_fp);
		}
		m_fp = fopen(m_file_name,m_mode);
	}
	else
	{
		if(m_file_buffer)
			delete m_file_buffer;
		m_file_buffer = new char[m_file_length + 1];
		fread(m_file_buffer,m_file_length,1,m_fp);
		m_file_buffer[m_file_length] = 0;
	}
}

//*****************************************************************************
//	函数说明：	以16进制dump内存数据，写入到文件
//	函数原型：	void dump_data(BYTE *buffer,int length,char *type)
//  参数：      buffer数据地址、length长度、type类型
//  返回值：    无
//  用法：		用于记录内存数据
//*****************************************************************************
void zxu::files::dump_data(BYTE *buffer, int length, char *type)
{
    std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	fprintf(m_fp,"[%s][长度：%d]\n",type,length);
	for(int i = 0;i < length;)
	{
		fprintf(m_fp,"%02X ",*(buffer+i));
		i++;
		if(i%16 == 0)
			fprintf(m_fp,"\n");
	}
	fprintf(m_fp,"\n\n");
}

//*****************************************************************************
//	函数说明：	写入到文件 (当写入缓冲区满时再写入文件)
//	函数原型：	void dump_data(BYTE *buffer,int length,char *type)
//  参数：      buffer数据地址、length长度、type类型
//  返回值：    无
//  用法：		用于记录内存数据
//*****************************************************************************
void zxu::files::log_printf(char *ctext, ...)
{
    std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	char temp_buf[MAX_LOG_BUF_LEN] = {0};
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf_s(temp_buf,"[%d:%d:%d:%d] ",time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
	va_list arg_list;
	va_start(arg_list,ctext);
	vsprintf(temp_buf+strlen(temp_buf),ctext,arg_list);
	va_end(arg_list);

	int current_log_len = strlen(temp_buf);
	if(current_log_len >= MAX_LOG_BUF_LEN)
	{
		fprintf(m_fp,m_printf_buf);
		m_buf_len = 0;
		fprintf(m_fp,temp_buf);
	}
	if (m_buf_len + current_log_len > MAX_LOG_BUF_LEN)
	{
		fprintf(m_fp,m_printf_buf);
		m_buf_len = current_log_len;
		memcpy_s(m_printf_buf,MAX_LOG_BUF_LEN,temp_buf,current_log_len);
	}
	else
	{
		memcpy_s(m_printf_buf+m_buf_len,MAX_LOG_BUF_LEN-m_buf_len,temp_buf,current_log_len);
		m_buf_len += current_log_len;
	}
}


//*****************************************************************************
//	函数说明：	64bit的位换位
//	函数原型：	DWORD64 bswap_64(DWORD64 value)
//  参数：      value需要换位的值
//  返回值：    返回换位之后的值
//  用法：		
//*****************************************************************************
uint64_ zxu::charset::bswap_64(uint64_ value)
{
    return (((uint64_)bswap_32(value)) << 32) | bswap_32((value >> 32));;
}

//*****************************************************************************
//	函数说明：	32bit换位
//	函数原型：	同上
//  参数：      同上
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
ulong_ zxu::charset::bswap_32(ulong_ value)
{
	_asm mov eax, value
		_asm bswap eax
}

//*****************************************************************************
//	函数说明：	16bit换位
//	函数原型：	同上
//  参数：      同上
//  返回值：    同上
//  用法：		同上
//*****************************************************************************
ushort_ zxu::charset::bswap_16(ulong_ value)
{
	_asm mov eax, value
		_asm and eax, 0xFFFF
		_asm xchg ah, al
}

//*****************************************************************************
//	函数说明：	内存中的字符串转换为输出文本格式，如内存字符串数据3aA-> 33 61 41
//	函数原型：	ULONG bin_to_hex(char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
//  参数：      hlen每行显示个数，ktext分隔符
//  返回值：    
//  用法：		
//*****************************************************************************
ULONG zxu::charset::bin_to_hex(const char *src, ULONG len, char * out, bool mode, char *ktext, int hlen)
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
//	函数说明：	内存数据文本格式转换为内存字符串数据，如内存数据的文本格式33 61 41 -> 3aA
//	函数原型：	ULONG hex_to_bin(char *src, ULONG len, char *out)
//  参数：      
//  返回值：    
//  用法：		
//*****************************************************************************
ULONG zxu::charset::hex_to_bin(const char *src, ULONG len, char *out)
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
		else if ((!len && !num) || (len && ti >= len) ) 
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
//	函数说明：	ANSI转换为UNICODE字符集
//	函数原型：	int ans_to_uni(LPSTR src_str, LPWSTR dst_str, int len)
//  参数：      src_str 需转换的ANSI字符串、dst_str转换之后的UNICODE字符集、len长度
//  返回值：    返回转换之后的长度
//  用法：		
//*****************************************************************************
int zxu::charset::ans_to_uni(const char* src_str, wchar_t* dst_str, int len)
{
	if (!len) len = strlen(src_str) + 1;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src_str, -1, dst_str, len);
	return len = lstrlenW(dst_str) * 2;
}

//*****************************************************************************
//	函数说明：	同上相反
//	函数原型：	
//  参数：      
//  返回值：    
//  用法：		
//*****************************************************************************
int zxu::charset::uni_to_ans(const wchar_t* src_str, char* dst_str, int len)
{
	if (!len) len = lstrlenW(src_str);
	len = WideCharToMultiByte(936, 0, src_str, len, dst_str, len * 2, 0, 0);
	dst_str[len]=0;
	return len;
}


//*****************************************************************************
//	函数说明：	UTF-8字符集转换为ANSI字符集
//	函数原型：	int utf8_to_ans(LPSTR src_str,LPSTR dst_str,int len)
//  参数：      
//  返回值：    
//  用法：		
//*****************************************************************************
int zxu::charset::utf8_to_ans(const char* src_str, char* dst_str, int len)
{
	int	tlen;
	LPWSTR *tstr;
	if (!len)	len=lstrlenA(src_str);
	tstr = (LPWSTR *)malloc(len*2);
	tlen = MultiByteToWideChar(CP_UTF8, 0, src_str, len,(LPWSTR)tstr, len*2);
	len = WideCharToMultiByte(936, 0, (LPCWSTR)tstr, tlen, (LPSTR)dst_str, len*2, 0, 0);
	free(tstr);
	dst_str[len]=0;
	return len;
}

//UTF-8
//*****************************************************************************
//	函数说明：	
//	函数原型：	
//  参数：      
//  返回值：    
//  用法：		
//*****************************************************************************
int zxu::charset::ans_to_utf8(const char* src_str, char* dst_str, int len)
{
	int	tlen;
	LPWSTR *tstr;
	if (!len)	len=lstrlenA(src_str);
	tstr=(LPWSTR *)malloc(len*2);
	tlen=MultiByteToWideChar(936, 0, src_str, len, (LPWSTR)tstr, len*2);
	len=WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)tstr, tlen, (LPSTR)dst_str, len*2, 0, 0);
	free(tstr);
	dst_str[len]=0;
	return len;
}

// ANSI转UTF-8(c++11)        亦可使用s_to_ws()配合ws_to_utf8()
const std::string zxu::charset::ans_to_utf8(std::string const &a_str)
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

// UTF-8转ANSI(c++11)
const std::string zxu::charset::utf8_to_ans(const std::string &u_str)
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

const std::string zxu::charset::ws_to_s(const std::wstring& src)
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
	mbstate_t out_state = {0};
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

const std::wstring zxu::charset::s_to_ws(const std::string& src)
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

const std::string zxu::charset::ws_to_utf8(const std::wstring& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(src);
}

const std::wstring zxu::charset::utf8_to_ws(const std::string& src)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
    return conv.from_bytes(src);
}


const int MaxKeyLen = 0x100;
bool zxu::cfg_reg_reader::read_from_reg(HKEY reg_key_root, std::string item_name)
{
    reg_key_root_ = reg_key_root;
    reg_item_name_ = item_name;
    HKEY tmp_key_handel;

    if (RegOpenKeyExA(reg_key_root, item_name.c_str(), 0, KEY_READ, &tmp_key_handel) != ERROR_SUCCESS)
    {
        return false;
    }
    char    tmp_subkey_buff[MaxKeyLen] = { 0 };			// buffer for subkey name
    DWORD   tmp_name_len;								// size of name string 
    char    tmp_class_buff[MAX_PATH] = "";		        // buffer for class name 
    DWORD   tmp_class_len = MAX_PATH;					// size of class string 
    DWORD   tmp_subkey_count = 0;						// number of subkeys 
    DWORD   tmp_longest_subkey_len;					    // longest subkey size 
    DWORD   tmp_longest_class_len;						// longest class string 
    DWORD   tmp_values_count;							// number of values for key 
    DWORD   tmp_longest_value_name;					    // longest value name 
    DWORD   tmp_longest_value_data;					    // longest value data 
    DWORD   tmp_security_descriptor;					// size of security descriptor 
    FILETIME tmp_last_write_time;						// last write time 

    DWORD tmp_ret_code = 0;

    // Get the class name and the value count. 
    tmp_ret_code = RegQueryInfoKeyA(tmp_key_handel, tmp_class_buff, &tmp_class_len, NULL, &tmp_subkey_count, &tmp_longest_subkey_len,
        &tmp_longest_class_len, &tmp_values_count, &tmp_longest_value_name, &tmp_longest_value_data, &tmp_security_descriptor, &tmp_last_write_time);

    std::lock_guard<std::recursive_mutex> tmp_lock(reg_list_lock_);
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    // 子项暂不枚举
    reg_subkey_list_.clear();
    if (tmp_subkey_count > 0)
    {
        for (DWORD i = 0; i<tmp_subkey_count; i++)
        {
            tmp_name_len = MaxKeyLen;
            tmp_ret_code = RegEnumKeyExA(tmp_key_handel, i, tmp_subkey_buff, &tmp_name_len, NULL, NULL, NULL, &tmp_last_write_time);
            if (tmp_ret_code != ERROR_SUCCESS)
            {
                RegCloseKey(tmp_key_handel);
                return false;
            }
            reg_subkey_list_.push_back(tmp_subkey_buff);
        }
    }

    // Enumerate the key values. 
    char tmp_value_name[MaxKeyLen] = { 0 };
    DWORD tmp_value_len = MaxKeyLen;
    DWORD tmp_data_type, tmp_data_size;
    BYTE tmp_value_data[MaxKeyLen * 5] = { 0 };
    reg_key_value_list_.clear();
    if (tmp_values_count)
    {
        for (DWORD i = 0; i<tmp_values_count; i++)
        {
            tmp_value_len = MaxKeyLen;
            tmp_value_name[0] = '\0';
            tmp_ret_code = RegEnumValueA(tmp_key_handel, i, tmp_value_name, &tmp_value_len, NULL, NULL, NULL, NULL);
            if (tmp_ret_code != ERROR_SUCCESS)
            {
                RegCloseKey(tmp_key_handel);
                return false;
            }

            tmp_value_data[0] = 0x00;
            tmp_data_size = MaxKeyLen * 5;
            tmp_ret_code = RegQueryValueExA(tmp_key_handel, tmp_value_name, NULL, &tmp_data_type, tmp_value_data, &tmp_data_size);
            if (tmp_ret_code != ERROR_SUCCESS)
            {
                RegCloseKey(tmp_key_handel);
                return false;
            }
            if (tmp_data_type == REG_DWORD)
            {
                DWORD tmp_int_value = *(DWORD *)tmp_value_data;
                std::stringstream tmp_ss;
                tmp_ss << tmp_int_value;
                std::string tmp_int_str = tmp_ss.str();
                reg_key_value_list_[tmp_value_name] = tmp_int_str;
                continue;
            }
            reg_key_value_list_[tmp_value_name] = (char *)tmp_value_data;
        }
    }
    RegCloseKey(tmp_key_handel);
    return true;
}

bool zxu::cfg_reg_reader::read_from_cfg(std::string file_name)
{
    cfg_file_name_ = file_name;
    std::fstream tmp_file;
    tmp_file.open(file_name, std::ios::in);
    if (!tmp_file.is_open())
        return false;
    if (tmp_file.bad())
        return false;
    std::vector<std::string> tmp_lines;
    std::string tmp_cur_line;
    while (!tmp_file.eof() && tmp_file.good())
    {
        std::getline(tmp_file, tmp_cur_line);
        if (tmp_cur_line.empty())
            continue;
        if (tmp_cur_line.size() >= 1 && (tmp_cur_line[0] == '#' || tmp_cur_line[0] == ';'))
            continue;
        tmp_lines.push_back(tmp_cur_line);
    }
    for (auto tmp_iter : tmp_lines)
    {
        _parse_cfg_line(tmp_iter);
    }
    tmp_file.close();
    return true;
}

std::string zxu::cfg_reg_reader::get_value_from_reg(std::string key)
{
    std::lock_guard<std::recursive_mutex> tmp_lock(reg_list_lock_);
    if (reg_key_value_list_.find(key) == reg_key_value_list_.end())
        return "";
    return reg_key_value_list_[key];
}

std::string zxu::cfg_reg_reader::get_value_from_cfg(std::string section_name, std::string key_name)
{
    std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
    auto tmp_app_iter = cfg_section_list_.find(section_name);
    if (tmp_app_iter == cfg_section_list_.end())
        return "";
    auto tmp_key_iter = tmp_app_iter->second.find(key_name);
    if (tmp_key_iter == tmp_app_iter->second.end())
        return "";
    return tmp_key_iter->second;
}

std::string zxu::cfg_reg_reader::get_value_from_cfg(std::string section_name, std::string key_name, const std::string& defaut_value)
{
    std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
    auto tmp_app_iter = cfg_section_list_.find(section_name);
    if (tmp_app_iter == cfg_section_list_.end())
        return defaut_value;
    auto tmp_key_iter = tmp_app_iter->second.find(key_name);
    if (tmp_key_iter == tmp_app_iter->second.end())
        return defaut_value;
    return tmp_key_iter->second;
}

bool zxu::cfg_reg_reader::read_str_from_cfg(std::string file_name, std::string type, std::string key, char *out_buf,std::size_t buff_len)
{
    if (0 >= GetPrivateProfileStringA(type.c_str(), key.c_str(), "", out_buf, buff_len, file_name.c_str()))
        return false;
    return true;
}

bool zxu::cfg_reg_reader::read_str_from_cfg(std::string file_name, std::string type, std::string key, std::string &out_str)
{
    out_str.assign(0x100, 0);
    if (0 >= GetPrivateProfileStringA(type.c_str(), key.c_str(), "", (char *)out_str.data(), out_str.size(), file_name.c_str()))
        return false;
    return true;
}

bool zxu::cfg_reg_reader::read_int_from_cfg(std::string file_name, std::string type, std::string key, int& out_data)
{
    out_data = GetPrivateProfileIntA(type.c_str(), key.c_str(), -1, file_name.c_str());

    if (out_data == -1)
        return false;

    return true;
}

void zxu::cfg_reg_reader::_parse_cfg_line(std::string &line)
{
    if (line.empty())
        return;
    // 去掉左右空格和去掉左右制表符
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);

    //// 去掉字符串中所有的空格和制表格
    //int j = 0;
    //for (int i = 0; i < line.size(); i++)
    //{
    //    if (std::isspace((unsigned char)line[i]))
    //    {
    //        continue;
    //    }
    //    line[j] = line[i];
    //    j++;
    //}
    //line.erase(j, line.size() - j);

    std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
    auto tmp_pos = line.rfind(']');
    if (tmp_pos != std::string::npos)
    {
        if (tmp_pos != 1 && line.find('[') == 0)
        {
            std::string tmp_section = line.substr(1, tmp_pos - 1);
            tmp_section.erase(0, tmp_section.find_first_not_of(" \t"));
            tmp_section.erase(tmp_section.find_last_not_of(" \t") + 1);
            cfg_cur_section_ = tmp_section;
            cfg_section_list_[tmp_section];
        }
    }
    else
    {
        tmp_pos = line.find('=');
        if (tmp_pos != std::string::npos)
        {
            std::string tmp_key, tmp_value;
            tmp_key = line.substr(0, tmp_pos);
            tmp_value = line.substr(tmp_pos + 1, line.size());
            tmp_key.erase(0, tmp_key.find_first_not_of(" \t"));
            tmp_key.erase(tmp_key.find_last_not_of(" \t") + 1);
            tmp_value.erase(0, tmp_value.find_first_not_of(" \t"));
            tmp_value.erase(tmp_value.find_last_not_of(" \t") + 1);
            cfg_section_list_[cfg_cur_section_].insert(std::map<std::string, std::string>::value_type(tmp_key, tmp_value));
        }
    }
}