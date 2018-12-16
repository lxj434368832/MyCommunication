#include "log_files.h"
#include "charset.h"

//#include <cstdarg>          // ������va_list���

#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE (1)
#endif
#pragma warning(disable : 4996)
#endif

//*****************************************************************************
//	����˵����	��ȡ�ļ���С
//	����ԭ�ͣ�	long get_file_size(FILE *fp)
//  ������      �ļ����fp
//  ����ֵ��    �����ļ���С
//  �÷���		��ȡĳ���ļ��Ĵ�С
//*****************************************************************************
long log_files::get_file_size()
{
	long curpos = 0, length = 0;
	curpos = ftell(m_fp);
	fseek(m_fp, 0L, SEEK_END);
	length = ftell(m_fp);
	fseek(m_fp, curpos, SEEK_SET);
	return length;
}


//*****************************************************************************
//	����˵����	��ȡ�ļ�
//	����ԭ�ͣ�	long file_read_string(char *filename, CString &out)
//  ������      file_name�ļ�����out_buf�ļ��������buf
//  ����ֵ��    �����ļ���С
//  �÷���		
//*****************************************************************************
long log_files::file_read_string(char *file_name, std::string &out_buf)
{
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		return 0;
	long len = get_file_size();
	out_buf.resize(len);
	fread((void *)out_buf.data(), len, 1, fp);
	fclose(fp);
	return len;
}


//*****************************************************************************
//	����˵����	��ȡ�ļ�
//	����ԭ�ͣ�	char *zxu::FileRead(char *file_name, char *fbuf, int &len)
//  ������      �ļ���file_name������ڴ�fbuf���������len)
//  ����ֵ��    ��������ڴ�
//  �÷���		�������fbufΪNULL��������ڴ棬����Ҫ���ⲿ������ͷ��ڴ�
//*****************************************************************************
char* log_files::file_read(char *file_name, char *fbuf, int &len)
{
	FILE *fp = fopen(file_name, "rb");
	if (!fp) return 0;
	if (!len) len = get_file_size();
	if (!fbuf) fbuf = (char *)malloc(len + 1);
	fread(fbuf, len, 1, fp);
	fbuf[len] = 0;
	fclose(fp);
	return fbuf;
}


//*****************************************************************************
//	����˵����	�����ļ�
//	����ԭ�ͣ�	bool file_save(char *filename, char *fbuf, int len)
//  ������      file_name�����ļ�����fbuf�ڴ�Ρ�len����
//  ����ֵ��    ��ȷ����true�����򷵻�false
//  �÷���		
//*****************************************************************************
bool log_files::file_save(char *file_name, char *fbuf, int len)
{
	FILE *fp = fopen(file_name, "ab+");
	if (!fp) return false;
	fwrite(fbuf, 1, len, fp);
	fclose(fp);
	return true;
}


//*****************************************************************************
//	����˵����	�������� LOG  ��Сʱ���ļ���¼
//	����ԭ�ͣ�	bool save_mem_log(char * f_name,char * sstr,char * src,int len)
//  ������      f_nameȫ·���ļ�����sstr��¼˵����src��Ҫ��¼���ݵ�ָ�롢len����
//  ����ֵ��    true
//  �÷���		
//*****************************************************************************
bool log_files::save_mem_log(char * f_name, char * sstr, char * src, int len)
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
//	����˵����	�����ı�LOG  ��Сʱ���ļ���¼
//	����ԭ�ͣ�	bool save_log(char * name,char * ctext,...)
//  ������      f_name�ļ�����ctext��������
//  ����ֵ��    true
//  �÷���		
//*****************************************************************************
bool log_files::save_log(char * f_name, char * ctext, ...)
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
//	����˵����	�ļ�������Ĵ������Ĺ��캯��
//	����ԭ�ͣ�	
//  ������      file_name�ļ�����modeΪ�����ļ��ĸ�ʽ��name_type��ʾ����־�ļ����Ƶ�������ʽ(��0���ļ���Ϊʱ��xx)
//  ����ֵ��    ��
//  �÷���		�������Ĵ�������			
//*****************************************************************************
log_files::log_files(const char * file_name, const char * mode, int name_type)
{
	m_file_length = 0;
	m_file_buffer = NULL;
	memset(m_mode, 0, sizeof(m_mode));
	memset(m_file_name, 0, MAX_PATH);
	memset(m_printf_buf, 0, MAX_LOG_BUF_LEN);
	m_buf_len = 0;
	memcpy_s(m_mode, sizeof(m_mode), mode, sizeof(mode));
	if (name_type)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		char t_file_name[MAX_PATH] = { 0 };
		char temp_path[MAX_PATH] = { 0 };
		sprintf(t_file_name, "(%d-%d-%d)%s", time.wYear, time.wMonth, time.wDay, file_name);
		GetModuleFileNameA(NULL, temp_path, MAX_PATH);
		(strrchr(temp_path, '\\'))[1] = 0;
		strcat_s(temp_path, MAX_PATH, "Log");				// Log��־�ļ���
		DWORD resoult = GetFileAttributesA(temp_path);	// ����Ƿ���ڣ���������ڸ��ļ��б㽨��
		if (resoult == -1)
			CreateDirectoryA(temp_path, NULL);
		char temp_file_name[MAX_PATH] = { 0 };
		sprintf(temp_file_name, "%s\\%s", temp_path, t_file_name);
		memcpy_s(m_file_name, MAX_PATH, temp_file_name, strlen(temp_file_name));
		m_fp = fopen(temp_file_name, m_mode);
	}
	else
	{
		memcpy_s(m_file_name, MAX_PATH, file_name, strlen(file_name));
		m_fp = fopen(file_name, m_mode);
	}
	if (m_fp)
		read_file_buff_len();
}

//*****************************************************************************
//	����˵����	file_handler�����������
//	����ԭ�ͣ�	~file_handler()
//  ������      ��
//  ����ֵ��    ��
//  �÷���		
//*****************************************************************************
log_files::~log_files()
{
	if (m_buf_len)
		fprintf(m_fp, m_printf_buf);
	if (m_fp)
		fclose(m_fp);
	if (m_file_buffer)
		delete m_file_buffer;
	m_file_buffer = NULL;
}

//*****************************************************************************
//	����˵����	��ӡ������ļ�
//	����ԭ�ͣ�	void dbg_print_to_file(char * ctext,...)
//  ������      ctext�����б�
//  ����ֵ��    ��
//  �÷���		
//*****************************************************************************
void log_files::dbg_print_to_file(char * ctext, ...)
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	va_list arg_list;
	va_start(arg_list, ctext);
	vfprintf(m_fp, ctext, arg_list);
	va_end(arg_list);
}

//*****************************************************************************
//	����˵����	��ȡ��ǰ�ļ���������ָ���ַ
//	����ԭ�ͣ�	char * get_file_buff()
//  ������      ��
//  ����ֵ��    �����ļ������ַ
//  �÷���		�ڲ�����
//*****************************************************************************
char* log_files::get_file_buff()
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	read_file_buff_len();
	return m_file_buffer;
}

//*****************************************************************************
//	����˵����	��ȡ�ļ���С������
//	����ԭ�ͣ�	void read_file_buff_len()
//  ������      ��
//  ����ֵ��    ��
//  �÷���		���ڿ�ʼ���ļ�ʱ
//*****************************************************************************
void log_files::read_file_buff_len()
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	m_file_length = get_file_size();
	if (strncmp(m_mode, "rb", 2) != 0)
	{
		fclose(m_fp);
		m_fp = fopen(m_file_name, "rb");
		if (m_fp)
		{
			if (m_file_buffer)
				delete m_file_buffer;
			m_file_buffer = new char[m_file_length + 1];
			fread(m_file_buffer, m_file_length, 1, m_fp);
			m_file_buffer[m_file_length] = 0;
			fclose(m_fp);
		}
		m_fp = fopen(m_file_name, m_mode);
	}
	else
	{
		if (m_file_buffer)
			delete m_file_buffer;
		m_file_buffer = new char[m_file_length + 1];
		fread(m_file_buffer, m_file_length, 1, m_fp);
		m_file_buffer[m_file_length] = 0;
	}
}

//*****************************************************************************
//	����˵����	��16����dump�ڴ����ݣ�д�뵽�ļ�
//	����ԭ�ͣ�	void dump_data(BYTE *buffer,int length,char *type)
//  ������      buffer���ݵ�ַ��length���ȡ�type����
//  ����ֵ��    ��
//  �÷���		���ڼ�¼�ڴ�����
//*****************************************************************************
void log_files::dump_data(BYTE *buffer, int length, char *type)
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	fprintf(m_fp, "[%s][���ȣ�%d]\n", type, length);
	for (int i = 0; i < length;)
	{
		fprintf(m_fp, "%02X ", *(buffer + i));
		i++;
		if (i % 16 == 0)
			fprintf(m_fp, "\n");
	}
	fprintf(m_fp, "\n\n");
}

//*****************************************************************************
//	����˵����	д�뵽�ļ� (��д�뻺������ʱ��д���ļ�)
//	����ԭ�ͣ�	void dump_data(BYTE *buffer,int length,char *type)
//  ������      buffer���ݵ�ַ��length���ȡ�type����
//  ����ֵ��    ��
//  �÷���		���ڼ�¼�ڴ�����
//*****************************************************************************
void log_files::log_printf(char *ctext, ...)
{
	std::lock_guard<std::recursive_mutex> locks(m_file_lock);
	if (!m_fp)
		return;
	char temp_buf[MAX_LOG_BUF_LEN] = { 0 };
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf_s(temp_buf, "[%d:%d:%d:%d] ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	va_list arg_list;
	va_start(arg_list, ctext);
	vsprintf(temp_buf + strlen(temp_buf), ctext, arg_list);
	va_end(arg_list);

	int current_log_len = strlen(temp_buf);
	if (current_log_len >= MAX_LOG_BUF_LEN)
	{
		fprintf(m_fp, m_printf_buf);
		m_buf_len = 0;
		fprintf(m_fp, temp_buf);
	}
	if (m_buf_len + current_log_len > MAX_LOG_BUF_LEN)
	{
		fprintf(m_fp, m_printf_buf);
		m_buf_len = current_log_len;
		memcpy_s(m_printf_buf, MAX_LOG_BUF_LEN, temp_buf, current_log_len);
	}
	else
	{
		memcpy_s(m_printf_buf + m_buf_len, MAX_LOG_BUF_LEN - m_buf_len, temp_buf, current_log_len);
		m_buf_len += current_log_len;
	}
}
