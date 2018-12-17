#ifndef SERVICE_INSTALLER_H_  
#define SERVICE_INSTALLER_H_  
/************************************************************************
* function��������ز�����������װ��ж�ء���ʼ��ֹͣ����
* author :	������
* datetime��2018-04-12
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/

#include "ServiceBase.h"  

class ServiceOperate 
{
public:
	static bool Install(LPCTSTR lpServiceName, LPCTSTR lpSrvPath, DWORD dwStartType, DWORD dwErrorControlType);
	static bool Uninstall(LPCTSTR lpServiceName);
	static BOOL IsInstalled(LPCTSTR lpServiceName);
	static bool Start(LPCTSTR lpServiceName);
	static bool Stop(LPCTSTR lpServiceName);
	static void ReconfigureService(LPSTR lpServiceName, LPSTR lpDesc);

private:
	ServiceOperate() {}
	static BOOL StopDependentServices(SC_HANDLE schSCManager, SC_HANDLE schService);
};

#endif // SERVICE_INSTALLER_H_