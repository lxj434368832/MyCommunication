#pragma once

#include <windows.h>
#include <mutex>

#define MAX_LOG_BUF_LEN 0x2000

class log_files
{
private:
	std::recursive_mutex m_file_lock;
	FILE *	m_fp;
	long	m_file_length;
	char *	m_file_buffer;
	char	m_mode[0x10];
	char	m_file_name[MAX_PATH];
	char	m_printf_buf[MAX_LOG_BUF_LEN];		// printf����
	int		m_buf_len;							// printf���������ݳ���

public:
	log_files(const char * file_name, const char * mode, int name_type = 0);
	~log_files();

	log_files(log_files const&) = delete;
	log_files& operator = (log_files const&) = delete;

public:
	void  dbg_print_to_file(char * ctext, ...);
	void  read_file_buff_len();
	char *get_file_buff();
	long get_file_size();                                           // ��ȡ�ļ���С
	void  dump_data(BYTE *buffer, int length, char *type);
	void  log_printf(char *ctext, ...);							    // ��д��������ʱ��д���ļ�
	long file_read_string(char *file_name, std::string &out_buf);   // ��ȡ�ļ���out_buf
	char *file_read(char *file_name, char *out_buf, int &len);      // ��ȡ�ļ���out_buf
	static bool file_save(char *file_name, char *fbuf, int len);           // �������ݵ��ļ�
	static bool save_mem_log(char * f_name, char * sstr, char * src, int len); // �������ݵ���־
	static bool save_log(char * f_name, char * ctext, ...);                    // �����ı�����־
};
