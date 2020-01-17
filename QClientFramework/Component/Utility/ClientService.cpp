#include "ClientService.h"
#include <windows.h>
#include <DbgHelp.h>
#include <time.h>
#include <QMessageBox>
#include <QApplication>

#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "version.lib")

ClientService::ClientService()
{
}


ClientService::~ClientService()
{
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	char dumpFile[256] = { 0 };
	time_t now;
	struct tm *t_tm;
	time(&now);
	t_tm = localtime(&now);
	sprintf_s(dumpFile, sizeof(dumpFile), "dump\\WPOClient%4d%02d%02d%02d%02d%02d.dmp",
		t_tm->tm_year + 1900,
		t_tm->tm_mon + 1,
		t_tm->tm_mday,
		t_tm->tm_hour,
		t_tm->tm_min,
		t_tm->tm_sec);

	//���� Dump �ļ�  
	HANDLE hDumpFile = ::CreateFileA(dumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		//Dump��Ϣ  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;
		//д��Dump�ļ�����  
		::MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
		::CloseHandle(hDumpFile);
	}

	//���ﵯ��һ������Ի����˳�����  
	EXCEPTION_RECORD* record = pException->ExceptionRecord;
	QString errCode(QString::number(record->ExceptionCode, 16)), errAdr(QString::number((uint)record->ExceptionAddress, 16)), errMod;
	QMessageBox::critical(NULL, QString::fromLocal8Bit("��ʽ����"), QString::fromLocal8Bit("<FONT size=4><div><b>���ڷ����Ĵ��󣬱�ʾ��ֿ��Ǹ��</b><br/></div>") +
		QString::fromLocal8Bit("<div>������룺%1</div><div>�����ַ��%2</div></FONT>").arg(errCode).arg(errAdr),
		QMessageBox::Ok);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ClientService::InstallAppCrashHandler()
{
#ifndef _DEBUG
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//ע���쳣������  
#endif // !_DEBUG

}

bool ClientService::IsApplicationRun(std::string strAppName)
{
	HANDLE hMutex = CreateMutexA(NULL, true, strAppName.data());
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		::CloseHandle(hMutex);
		QString qstrMsg = QString("<font size='12' color='red'>%1</font>").arg(QString::fromLocal8Bit("�Ѿ�������һ������,��Ҫ�ȹرճ���"));
		QMessageBox::information(nullptr, QString::fromLocal8Bit("ϵͳ��Ϣ"), qstrMsg
				, QString::fromLocal8Bit("ȷ��"));
		return true;
	}
	return false;
}

QString ClientService::GetAppVersion()
{
	QString fullName = QApplication::applicationFilePath();

	QString result = "";
	char* pData = nullptr;
	do
	{
		//����ļ�������Ϣ
		DWORD dwLen = GetFileVersionInfoSize(fullName.toStdWString().c_str(), 0);
		if (0 == dwLen)
		{
			break;
		}
		pData = new char[dwLen + 1];
		BOOL bSuccess = GetFileVersionInfo(fullName.toStdWString().c_str(), 0, dwLen, pData);
		if (!bSuccess)
		{
			break;
		}
		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;
		LPVOID lpBuffer = nullptr;
		UINT uLen = 0;
		//������Ժʹ���ҳ(language and code page)
		bSuccess = VerQueryValue(pData, (TEXT("\\VarFileInfo\\Translation")), (LPVOID*)&lpTranslate, &uLen);
		if (!bSuccess)
		{
			break;
		}
		QString str1, str2;
		str1.setNum(lpTranslate->wLanguage, 16);
		str2.setNum(lpTranslate->wCodePage, 16);
		str1 = "000" + str1;
		str2 = "000" + str2;
		//����ļ��汾��Ϣ
		QString verPath = "\\StringFileInfo\\" + str1.right(4) + str2.right(4) + "\\FileVersion";
		bSuccess = VerQueryValue(pData, (verPath.toStdWString().c_str()), &lpBuffer, &uLen);
		if (!bSuccess)
		{
			break;
		}
		result = QString::fromUtf16((const unsigned short int *)lpBuffer);
	} while (false);
	if (nullptr != pData)
	{
		delete pData;
	}
	return result;
}
