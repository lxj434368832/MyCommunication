//**********************************************************************
//*		说明：此工具箱包含了文件处理、socket初始化、临界区锁初始化、
//*			  钩子、字符集转换、某些系统公共函数的包装函数、IAT、
//*			  TCP/UDP连接函数等
//*		作者：PotalXu、CheLi
//*		日期：2012.01.01（修改整合于2016.01.11 by PotalXu）
//**********************************************************************
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <cstdarg>          // 定义了va_list相关

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#define MAX_LOG_BUF_LEN 0x2000

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
typedef int     sock_fd;
#else
typedef int     sock_fd;
#endif
#else
typedef int     sock_fd;
#endif

typedef unsigned char       ubyte_;
typedef unsigned short      ushort_;
typedef unsigned long       ulong_;
typedef unsigned long long  uint64_;
typedef unsigned int        uint32_;
typedef char                byte_;
typedef short               int16_;
typedef int                 int32_;
typedef long                long_;
typedef long long           int64_;

namespace zxu
{
	class files;    	    // 文件类
	class init_socket;      // socket初始化类
    class winapi;           // 
    class sockets;          // socket相关函数
	class hook;             // HOOK类
	class charset;          // 字符类
    class cfg_reg_reader;   // 配置文件和注册表读取类

//***********系统包装函数**************//
#ifdef WIN32
    std::string get_module_path(HMODULE module, const char *f_name);        // 获取文件路径名
	HMODULE get_self_module_handle();                                       // 获得当前模块句柄包括dll模块
	bool is_process_a(HMODULE module, char *pname);                         // 判断模块名
	int get_num_of_processors(); // 获取PC的CPU数量
	//guid类型:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	std::string make_guid();
#endif
    bool is_time(time_t ms, time_t dms);                                    // 是否大于时间
    bool is_time_v(time_t &ms, time_t dms);                                 // 大于时间修改

    class sockets
    {
    public:
        sockets() = default;
    public:
        static int get_sock_name(sock_fd s, struct sockaddr_in *sock);                         // 
        static sock_fd tcp_link(const std::string &ip, int port);                       // TCP连接
        static sock_fd tcp_listen(const std::string &ip, ushort_ port);                 // TCP监听
        static sock_fd udp_link(const std::string &ip, int port, struct sockaddr_in &sock);    // UDP连接
        static sock_fd udp_listen(const std::string &ip, ushort_ port);                 // UDP监听
        static const char* inet_ntoa(unsigned long ip_addr);

        static int recv_to_end(sock_fd s, char *buffer, int length, int flag);       // recv函数的进一步包装
        static int send_to_end(sock_fd s, char *buffer, int length, int flag);       // 发送length的数据
    };

    class files
	{
	private:
		std::recursive_mutex m_file_lock;
		FILE *	m_fp;
		long	m_file_length;
		char *	m_file_buffer;
		char	m_mode[0x10];
		char	m_file_name[MAX_PATH];
		char	m_printf_buf[MAX_LOG_BUF_LEN];		// printf缓冲
		int		m_buf_len;							// printf缓冲中数据长度

    public:
        files(const char * file_name, const char * mode, int name_type = 0);
        ~files();

        files(files const&) = delete;
        files& operator = (files const&) = delete;
        
    public:
		void  dbg_print_to_file(char * ctext,...);
		void  read_file_buff_len();
		char *get_file_buff();
        long get_file_size();                                           // 获取文件大小
        void  dump_data(ubyte_ *data, int length, char *type);
		void  log_printf(char *ctext,...);							    // 当写缓冲区满时再写入文件
        long file_read_string(char *file_name, std::string &out_buf);   // 读取文件到out_buf
        char *file_read(char *file_name, char *out_buf, int &len);      // 读取文件到out_buf
        static bool file_save(char *file_name, char *fbuf, int len);           // 保存数据到文件
        static bool save_mem_log(char * f_name, char * sstr, char * src, int len); // 保存数据到日志
        static bool save_log(char * f_name, char * ctext, ...);                    // 保存文本到日志
	};

	class hook
	{
	private:
		FARPROC	api_proc;
        ubyte_	hook_hex[8];    //钩代码
        ubyte_	src_hex[8];     //源代码
	public:
        hook() = default;
        ~hook() = default;
    public:
		//HOOK api
        void hook_api(HINSTANCE dll_inst, char *api_name, FARPROC call_proc);
		//HOOK code
        void hook_code(FARPROC proc, FARPROC call_proc);
        void set();		//钩
        void revert();	//还原
        static int replace_iat(HANDLE process, const char *dll_name, const char *api_name, DWORD &call_code, DWORD hook_code);	//IAT Hook
	};

    class charset
	{
	public:
        charset(){};
        ~charset(){};
	public:
        static ULONG bin_to_hex(const char *src, ulong_ len, char * out, bool mode, char *ktext, int hlen);     // 与下函数相反
        static ULONG hex_to_bin(const char *src, ulong_ len, char *out);                                        // 内存数据格式转换为字符串，如内存数据0x33 0x61 0x41-> 3aA
        static int uni_to_ans(const wchar_t *src_str, char *dst_str, int len);                                  // UNICODE转ANSI
        static int ans_to_uni(const char *src_str, wchar_t *dst_str, int len);                                  // ANSI转UNICODE
        static int utf8_to_ans(const char *src_str, char *dst_str, int len);                                    // UTF-8转ANSI
        static int ans_to_utf8(const char *src_str, char *dst_str, int len);                                    // ANSI转UTF-8

        static const std::string ans_to_utf8(const std::string &a_str);                                         // ANSI转UTF-8(c++11)
        static const std::string utf8_to_ans(const std::string &u_str);                                         // UTF-8转ANSI(c++11)
        static const std::string ws_to_s(const std::wstring& src);
        static const std::wstring s_to_ws(const std::string& src);
        static const std::string ws_to_utf8(const std::wstring& src);
        static const std::wstring utf8_to_ws(const std::string& src);

        static uint64_ bswap_64(uint64_ value);                                                        // 数据位转换，下同
        static ulong_ bswap_32(ulong_ value);
        static ushort_ bswap_16(ulong_ value);
	};

    class cfg_reg_reader
	{
    private:
        // 注册表相关
        HKEY								reg_key_root_;
        std::string							reg_item_name_;			// 注册表中的项
        std::vector<std::string>			reg_subkey_list_;		// 该项下子项的表单
        std::map<std::string, std::string>	reg_key_value_list_;	// 上述项对应的key和value
        std::recursive_mutex				reg_list_lock_;			// 链表锁

        // 配置文件相关
        std::string							cfg_file_name_;
        std::map<std::string, std::map<std::string, std::string>> cfg_section_list_;
        std::string							cfg_cur_section_;
        std::recursive_mutex				cfg_list_lock_;

    public:
        cfg_reg_reader() = default;
        ~cfg_reg_reader() = default;

        bool read_from_reg(HKEY reg_key_root, std::string item_name);
        bool read_from_cfg(std::string file_name);

        std::string get_value_from_reg(std::string key);
        std::string get_value_from_cfg(std::string section_name, std::string key_name);
        std::string get_value_from_cfg(std::string section_name, std::string key_name, const std::string& defaut_value);
        static bool read_str_from_cfg(std::string file_name, std::string type, std::string key, char *out_buf,std::size_t buff_len);
        static bool read_str_from_cfg(std::string file_name, std::string type, std::string key, std::string &out_str);
        static bool read_int_from_cfg(std::string file_name, std::string type, std::string key, int& out_data);
    private:
        void _parse_cfg_line(std::string &line);
	};
}
