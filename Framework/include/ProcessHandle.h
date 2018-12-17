/************************************************************************
* function:���̸�������
* author :	������
* datetime:2018-04-03
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/
#pragma once

#include <windows.h>
#include <string>
#include <list>
#include <map>

class ProcessHandle
{
public:
	/*************************************************************************
	* function:		���ģ��·��
	* param:		moduleģ���������Ϊ�գ����ȡ��ǰ���̵�·��
	* return:		��ǰģ���Ӧ��·��
	*************************************************************************/
	static std::string GetModulePath(HMODULE module);
	/*************************************************************************
	* function:		ͨ���������ֻ�ȡPID
	* param pName:	in,����exe����
	* return:		��ǰ����Ӧ�Ľ���PID
	*************************************************************************/
	static DWORD GetProcessIDByName(LPCSTR pName);

	/*************************************************************************
	* function:			ͨ������ID�������Ƿ����
	* param dwProcessID:in,����PID
	* return:			���̾��
	*************************************************************************/
	static bool CheckProcessRunByPID(DWORD dwProcessID);

	/*************************************************************************
	* function:		ͨ���������ֻ�ȡPID�б����������������ͬ�ĳ���
	* param pName:	in,����exe����
	* return:		��ǰ����Ӧ�Ľ���PID�б�
	*************************************************************************/
	static std::list<DWORD> GetProcessIDListByName(LPCSTR pProcName);

	/*************************************************************************
	* function:			ͨ������ID��ȡ����·��
	* param dwProcessID:in,���̵�PID
	* param buffer:		out,���̵�·��
	*************************************************************************/
	static void GetProcessPathByPID(DWORD dwProcessID, LPSTR buffer);

	/*************************************************************************
	* function:		ͨ���������ֻ�ȡ����·��
	* param pName:	in,��������
	* param buffer:	out,���̵�·��
	*************************************************************************/
	static void GetProcessPathByName(LPCSTR pName, LPSTR buffer);

	/*************************************************************************
	* function:		ͨ����������ȡ���̵�Ŀ¼
	* param pName:	in,��������
	* param buffer:	out,���̵�Ŀ¼
	*************************************************************************/
	static void GetProcessDirByName(LPCSTR pName, LPSTR buffer);

	/*************************************************************************
	* function:		ͨ����������ȡ���̵�Ŀ¼�б�
	* param pName:	in,��������
	* param DirList:out,���̵�Ŀ¼�б�
	*************************************************************************/
	static void GetProcessDirListByName(LPCSTR pProcName, std::map<DWORD, std::string> &DirList);

	/*************************************************************************
	* function:			ͨ������ID��ȡ���̾��
	* param dwProcessID:in,����PID
	* return:			���̾��
	*************************************************************************/
	static HANDLE GetProcessHandleByPID(DWORD dwProcessID);

	/*************************************************************************
	* function:			ͨ����������ȡ���̾��
	* param pName:		in,������
	* return:			���̾��
	*************************************************************************/
	static HANDLE GetProcessHandleByName(LPCSTR pName);

	/*************************************************************************
	* function:			��ֹ����
	* param pName:		in,������
	* return:			�ɹ�����true��ʧ��false
	*************************************************************************/
	static bool  TerminateProcess(LPCSTR pName);

	/*************************************************************************
	* function:			ͨ�����̵�PID��ֹ����
	* param pName:		in,������
	* return:			�ɹ�����true��ʧ��false
	*************************************************************************/
	static bool  TerminateProcByPID(DWORD dwPID);

	/*************************************************************************
	* function:			��ֹ���̣�ʹ��nt�ں˺�����ֹ
	* param pName:		in,������
	* return:			�ɹ�����true��ʧ��false
	*************************************************************************/
	static bool NTTerminateProcess(LPCSTR lpName);

	/*************************************************************************
	* function:				��������
	* param strAppName:		in,Ӧ�ó�������
	* param strCmdline:		in,���ݸ�Ӧ�ó���������в���
	* param strCurDir:		in,Ӧ�ó���ĵ�ǰ����Ŀ¼
	* return:				�ɹ�����true,ʧ��false
	*************************************************************************/
	static bool CreateProcess(LPCSTR strAppName,LPSTR strCmdline,LPCSTR strCurDir);
	static bool CreateProcessByCmd(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);
	static BOOL CreateProcessTUser(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);
	static BOOL CreateProcessTKernel(LPCSTR strAppName, LPSTR strCmdline, LPCSTR strCurDir);

	static BOOL GetTokenByName(LPTSTR lpName, HANDLE &hToken);

	static BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpPrivilegeName, BOOL bEnablePrivilege);

protected:

	class MHandle
	{
	public:
		MHandle() :m_handle(NULL){}
		MHandle(HANDLE handle) : m_handle(handle) {}

		~MHandle()
		{
			if (m_handle)
			{
				::CloseHandle(m_handle);
			}
		}

		operator HANDLE()
		{
			return m_handle;
		}

		HANDLE* operator &(){ return &m_handle; }
		HANDLE& operator *() = delete;

	private:
		HANDLE m_handle;
	};
};

