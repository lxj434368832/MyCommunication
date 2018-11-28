#pragma once
/*************************************************************************
* function��iocpģ�Ͷ����ļ�����װ��ص�����������������ϵͳ���������
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "stdafx.h"
#include <MSWSock.h>

struct IOContext;

// ��Ҫ��װiocp��ص����ӡ����ա����ͺ���

class IOCPModule
{
public:
	~IOCPModule();
	static IOCPModule* Instance();

	SOCKET Socket();

	/*************************************************************************
	* function����socket�����ص�ַname
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int Bind(SOCKET s, const LPSOCKADDR name);

	/*************************************************************************
	* function��������������
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int Listen(SOCKET s, int iMaxUserCount);

	/*************************************************************************
	* function��������ɶ˿�
	* return:	�ɹ����ش����ľ����ʧ�ܷ���NULL
	*************************************************************************/
	HANDLE CreateIoCompletionPort();

	/*************************************************************************
	* function����socket�󶨵���ɶ˿�hIOCP��
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int BindIoCompletionPort(SOCKET socket, HANDLE hIOCP);
	
	/*************************************************************************
	* function�������ͻ��˵�����
	* return:	�ɹ�����0��ʧ�ܷ���WSAGetLastError()��ֵ
	*************************************************************************/
	int AcceptEx(SOCKET srvSocket, IOContext *pIO);

	void GetAcceptExSockaddrs(IOContext *pIO, LPSOCKADDR *client);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int ConnectEx(IOContext *pIO, const LPSOCKADDR name);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int DisconnectEx(IOContext *pIO);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int GetQueuedCompletionStatus(HANDLE hcp, IOContext **ppIOContext);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int PostQueuedCompletionStatus(HANDLE hCP, DWORD dwTransBytes, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOL);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int Send(IOContext *pIO);

	//�ɹ�����0�����ɹ�����GetLastError()��ֵ
	int Receive(IOContext *pIO);

private:
	IOCPModule();
	IOCPModule(const IOCPModule&);
	IOCPModule& operator=(const IOCPModule&);
	void LoadAllWSAFunction();

private:
	LPFN_CONNECTEX m_fnConnectEx = NULL;
	LPFN_ACCEPTEX m_fnAcceptEx = NULL;
	LPFN_DISCONNECTEX m_fnDisconnectEx = NULL;
	LPFN_GETACCEPTEXSOCKADDRS m_fnGetAcceptExSockaddrs = NULL;
};
