#pragma once
/*******************************************************************
* @��Ȩ��Ϣ��
* @�ļ����ƣ�LogFile.h
* @ժ    Ҫ����־��������ͷ�ļ�
* @��    �ߣ�������
* @��ǰ�汾��1.0.0
* @��    �ڣ�2019��5��21��
* @��    ע���̰߳�ȫ��־��
*******************************************************************/

#include <sstream>

class SingleLog
{
public:
	SingleLog();
	~SingleLog();
	void AddLog(char* format, ...);

	template <typename T>
	inline SingleLog& operator<<(const T& log)
	{
		m_strStream << log;
		return *this;
	}

	inline SingleLog& operator<<(std::ostream& (*log)(std::ostream&))
	{
		m_strStream << log;
		return *this;
	}

private:
	char	m_szLineLog[512];
	std::stringstream m_strStream;
};

class LogFileData;

class LogFile
{
public:
	LogFile();
	~LogFile();
	static LogFile* GetInstance()
	{
		return s_instance;
	}
	void AddLog(std::string &strLog);

	void Timeout(unsigned uTimerID);

private:
	void CheckFileName();
	void WriteLogThread();

	void WriteLog(std::string &strLog);

private:
	static LogFile* s_instance;
	LogFileData		*d;
};


#define Log() (SingleLog())<< "["<<__FUNCTION__<<":"<<__LINE__<< "]"
#define logm() Log()<<"<Info> "
#define logd() Log()<<"<Debug> "
#define logw() Log()<<"<Warn> "
#define loge() Log()<<"<Error> "

//���µ��÷�ʽ����Ϣ���Ȳ��ܳ���256
#define LOGM(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Info> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGD(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Debug> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGW(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Warn> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
#define LOGE(format, ...)  do{  SingleLog log; \
    log.AddLog("[%s:%d]<Error> ",__FUNCTION__,__LINE__);  \
    log.AddLog(format, ##__VA_ARGS__);}while(0)
