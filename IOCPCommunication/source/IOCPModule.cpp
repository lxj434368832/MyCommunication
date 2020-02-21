#include "..\include\IOCPModule.h"
#include "..\Framework\include\CommonDefine.h"
#include "..\include\stdafx.h"
#include <mstcpip.h>	//keeplive���
#include <ws2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Kernel32.lib")

IOCPModule::IOCPModule()
{
	WSADATA wsaData;
	int nResult;
	nResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

	char Assert[1];
	NO_ERROR != nResult && Assert[-0xffff];
	(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) && Assert[-0xffff];

	LoadAllWSAFunction();
}

IOCPModule::~IOCPModule()
{
	::WSACleanup();
}

IOCPModule* IOCPModule::Instance()
{
	static IOCPModule s_instance;
	return &s_instance;
}

int IOCPModule::GetProcessorCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

SOCKET IOCPModule::Socket()
{
	SOCKET sock = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	return sock;
	//return::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
}

int IOCPModule::Bind(SOCKET s, const LPSOCKADDR name)
{
	int iRet = 0;
	if (SOCKET_ERROR == ::bind(s, name, sizeof(SOCKADDR)))
	{
		iRet = WSAGetLastError();
		LOGM("bind socket:%d failed, error code:%d ", s, iRet);	//ʧ�ܿ����Ƕ˿ڱ�ռ��
	}
	return iRet;
}

int IOCPModule::Listen(SOCKET s, int iMaxUserCount)
{
	int iRet = 0;
	if (SOCKET_ERROR == ::listen(s, iMaxUserCount))
	{
		iRet = WSAGetLastError();
		LOGM("����socket:%dʧ�ܣ�������:%d", s, iRet);
	}
	return iRet;
}

HANDLE IOCPModule::CreateIoCompletionPort()
{
	return::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
}

int IOCPModule::BindIoCompletionPort(PER_SOCKET_CONTEXT *pSkContext, HANDLE hIOCP)
{
	int iRet = 0;
	if (NULL == ::CreateIoCompletionPort((HANDLE)pSkContext->m_socket, hIOCP, (DWORD_PTR)pSkContext, 0))
	{
		iRet = ::WSAGetLastError();
		LOGM("socket:%d�󶨵���ɶ˿�ʧ�ܣ������룺%d", pSkContext->m_socket, iRet);
	}
	return iRet;
}

int IOCPModule::AcceptEx(SOCKET listenSocket, PER_IO_CONTEXT *pIO)
{
	int iRet = 0;
	int iSockaddrSize = sizeof(SOCKADDR) + 16;
	DWORD dwBytes = 0;
	pIO->m_oprateType = EOP_ACCEPT;

	//if (false == m_fnAcceptEx(listenSocket,pIO->m_socket,pIO->m_wsaBuf.buf,pIO->m_wsaBuf.len - iSockaddrSize*2,
	//	iSockaddrSize,iSockaddrSize,&dwBytes,&pIO->m_overlapped))
	//Ϊ�˷�ֹ��Է������Ĺ���������ʱ���������ݣ����ӵ����������ء�
	if (false == m_fnAcceptEx(listenSocket, pIO->m_socket, pIO->m_wsaBuf.buf, 0,
			iSockaddrSize, iSockaddrSize, &dwBytes, &pIO->m_overlapped))
	{
		iRet = ::WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
		{
			LOGM("����sokcet:%d AcceptExʧ�ܣ�������:%d", pIO->m_socket, iRet);
		}
		else
			iRet = 0;
	}
	return iRet;
}

void IOCPModule::GetAcceptExSockaddrs(PER_IO_CONTEXT *pIO, LPSOCKADDR *lpAddr)
{
	LPSOCKADDR local = nullptr;
	int iAddrLen = sizeof(SOCKADDR);
	DWORD dwFlag = 0;
	int iRet = 0;
	m_fnGetAcceptExSockaddrs(pIO->m_wsaBuf.buf, pIO->m_wsaBuf.len - (iAddrLen + 16) * 2, iAddrLen, iAddrLen,
		&local, &iAddrLen, lpAddr, &iAddrLen);
}

std::string IOCPModule::GetIPAddress(PVOID pAddrBuf)
{
	char strIP[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, pAddrBuf, strIP, INET_ADDRSTRLEN);
}

bool IOCPModule::ParseIPAddress(std::string strIP, PVOID pAddrBuf)
{
	bool bRet = false;
	int iRet = inet_pton(AF_INET, strIP.c_str(), pAddrBuf);
	if (0 == iRet)
	{
		loge() << "IP:" << strIP.c_str() << "��ʽ����ȷ��";
	}
	else if (-1 == iRet)
	{
		loge() << "����IP��ַʧ�ܣ�������:" << ::GetLastError();
	}
	else
	{
		bRet = true;
	}
	return bRet;
}

int IOCPModule::ConnectEx(PER_IO_CONTEXT *pIO, const LPSOCKADDR name)
{
	int bRet = 0;
	DWORD dwLen = 0;
	pIO->m_oprateType = EOP_CONNECT;
	if (false == m_fnConnectEx(pIO->m_socket, name, sizeof(SOCKADDR),
			pIO->m_wsaBuf.buf, pIO->m_wsaBuf.len, &dwLen,&pIO->m_overlapped))
	{
		bRet = GetLastError();
		if (WSA_IO_PENDING != bRet)
		{
			LOGM("Ͷ��ConnectExʧ�ܣ������룺%d", bRet);
		}
		else
			bRet = 0;

	}
	return bRet;
}

int IOCPModule::DisconnectEx(PER_IO_CONTEXT *pIO)
{
	int iRet = 0;
	pIO->m_oprateType = EOP_DISCONNECT;
	if (false == m_fnDisconnectEx(pIO->m_socket,&pIO->m_overlapped, TF_REUSE_SOCKET,0))
	{
		iRet = WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
		{
			LOGM("����ʧ�ܣ������룺%d", iRet);
		}
		else
		{
			iRet = 0;
		}
	}
	return iRet;
}

int IOCPModule::PostQueuedCompletionStatus(HANDLE hCP, DWORD dwTransBytes, ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOL)
{
	int iRet = 0;
	if (false == ::PostQueuedCompletionStatus(hCP, dwTransBytes, dwCompletionKey, lpOL))
	{
		iRet = ::GetLastError();
		LOGM("����ʧ�ܣ������룺%d", iRet);
	}
	return iRet;
}

int IOCPModule::Send(PER_IO_CONTEXT *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;

	pIO->m_oprateType = EOP_SEND;
	if (SOCKET_ERROR == WSASend(pIO->m_socket, &pIO->m_wsaBuf, 1, nullptr, dwFlags, &pIO->m_overlapped, NULL))
	{
		iRet = WSAGetLastError();
		if (iRet != WSA_IO_PENDING)
			LOGM("��������ʧ�ܣ������룺%d", iRet);
		else
			iRet = 0;

	}
	return iRet;
}

int IOCPModule::Receive(PER_IO_CONTEXT *pIO)
{
	DWORD dwFlags = 0;
	int iRet = 0;
	pIO->m_oprateType = EOP_RECEIVE;
	if (SOCKET_ERROR == WSARecv(pIO->m_socket, &pIO->m_wsaBuf, 1, nullptr, &dwFlags, &pIO->m_overlapped, nullptr))
	{
		iRet = WSAGetLastError();
		if (WSA_IO_PENDING != iRet)
			LOGM("����ʧ�ܣ������룺%d", iRet);
		else
			iRet = 0;
	}
	return iRet;
}

bool IOCPModule::SetKeepLiveParam(PER_IO_CONTEXT *pIO)
{
	bool bKeepAlive = true;
	int nRet = setsockopt(pIO->m_socket, SOL_SOCKET, SO_KEEPALIVE,
		(char*)&bKeepAlive, sizeof(bKeepAlive));
	if (nRet == SOCKET_ERROR)
	{
		LOGM("setsockopt failed: %d/n", WSAGetLastError());
		return false;
	}

	// set KeepAlive parameter
	tcp_keepalive alive_in;
	alive_in.keepalivetime = 5000;  // 5s
	alive_in.keepaliveinterval = 1000; //1s,total time 1000*10(10s)
	alive_in.onoff = TRUE;
	unsigned long ulBytesReturn = 0;

	pIO->m_oprateType = EOP_KEEP_LIVE;
	nRet = WSAIoctl(pIO->m_socket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		nullptr, 0, &ulBytesReturn, &pIO->m_overlapped, NULL);
	if (nRet == SOCKET_ERROR)
	{
		LOGM("WSAIoctl failed: %d/n", WSAGetLastError());
		return false;
	}
	return true;
}

void IOCPModule::LoadAllWSAFunction()
{
	DWORD dwBytes = 0;
	GUID guid;
	char Assert[1];

	SOCKET socket = Socket();
	if (INVALID_SOCKET == socket)
	{
		LOGM( "����socketʧ��,�������ǣ�%d" ,WSAGetLastError());
		Assert[-0xffff];
	}

	guid = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnConnectEx, sizeof(m_fnConnectEx), &dwBytes, NULL, NULL))
	{
		LOGM("��ȡ����ָ��ʧ�ܣ������룺%d", WSAGetLastError());
	}

	guid = WSAID_ACCEPTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnAcceptEx, sizeof(m_fnAcceptEx), &dwBytes, NULL, NULL))
	{
		LOGM("��ȡ����ָ��ʧ�ܣ������룺%d", WSAGetLastError());
	}

	guid = WSAID_DISCONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnDisconnectEx, sizeof(m_fnDisconnectEx), &dwBytes, NULL, NULL))
	{
		LOGM("��ȡ����ָ��ʧ�ܣ������룺%d", WSAGetLastError());
	}

	guid = WSAID_GETACCEPTEXSOCKADDRS;
	if (SOCKET_ERROR == WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
		&m_fnGetAcceptExSockaddrs, sizeof(m_fnGetAcceptExSockaddrs), &dwBytes, NULL, NULL))
	{
		LOGM("��ȡ����ָ��ʧ�ܣ������룺%d", WSAGetLastError());
	}

	::closesocket(socket);
}
