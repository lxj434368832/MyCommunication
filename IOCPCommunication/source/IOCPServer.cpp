#include "..\include\IOCPDef.h"
#include "..\include\IOCPServer.h"
#include "..\include\IOCPModule.h"
#include "..\include\INetInterface.h"
#include "..\include\stdafx.h"
#include <assert.h>

IOCPServer::IOCPServer(INetInterface *pNet) : 
	IOCPBase(pNet)
{
	m_bStart = false;
	m_pListenSocketContext = nullptr;
}

IOCPServer::~IOCPServer()
{
}

bool IOCPServer::StartServer(USHORT nPort, unsigned dwMaxConnection, unsigned uThreadCount)
{
	if (false == InitIOCP(uThreadCount))
		return false;

	if (false == StartServerListen(nPort, dwMaxConnection))
	{
		//��ȫ�����߳��˳�
		for (unsigned i = 0; i < d->uThreadCount; i++)
		{
			IOCPModule::Instance()->PostQueuedCompletionStatus(d->hIOCompletionPort, 0, NULL, NULL);
		}

		for (unsigned i = 0; i < d->uThreadCount; i++)
		{
			if (d->aThreadList[i].joinable())
			{
				d->aThreadList[i].join();
			}
		}
		delete[] d->aThreadList;
		d->aThreadList = nullptr;

		delete[] d->aThreadList;
		d->aThreadList = nullptr;

		RELEASE_HANDLE(d->hIOCompletionPort);
		return false;
	}
	m_bStart = true;
	return true;
}

bool IOCPServer::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = d->rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
		{
			m_pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
			{
				LOGM("����Server����socketʧ�ܣ������룺%d", WSAGetLastError());
				break;
			}
		}

		//SO_REUSEADDR����ͬһport������ͬһ�������Ķ��ʵ��(�������)����ÿ��ʵ���󶨵�IP��ַ�ǲ�����ͬ�ġ�
		//int nOpt = 1;
		//setsockopt(m_pListenSocketContext->m_Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(nOpt));	//���õ�ַ

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if (IOCPModule::Instance()->Bind(m_pListenSocketContext->m_socket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(m_pListenSocketContext->m_socket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, d->hIOCompletionPort)) break;

		//Ͷ�ݽ��ܲ���
		for (unsigned i = 0; i < iMaxConnectCount; i++)
		{
			if (false == (bRet = PostAcceptEx(m_pListenSocketContext->m_socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		RELEASE_SOCKET(m_pListenSocketContext->m_socket);
		d->rscSocketContext.put(m_pListenSocketContext);
		m_pListenSocketContext = nullptr;
	}
	return bRet;
}

void IOCPServer::StopServer()
{
	if (false == m_bStart) return;
	m_bStart = false;

	logm() << "�رռ����˿ڡ�";
	RELEASE_SOCKET(m_pListenSocketContext->m_socket);
	d->rscSocketContext.put(m_pListenSocketContext);
	m_pListenSocketContext = nullptr;

	logm() << "ȡ����������socket��IO������";
	d->lckConnectList.lock();
	auto iter = d->mapConnectList.begin();
	while (iter != d->mapConnectList.end())
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			LOGM("�����б��е���Դ����ȷ,��Ӧ�ô���INVALID_SOCKET��");
			iter++;
		}
		else
		{
			if (EOP_ACCEPT == pSkContext->m_ReceiveContext.m_oprateType)
			{
				iter++;
			}
			else
			{
				::InterlockedExchange(&pSkContext->m_iDisconnectFlag, 1);

				//if (SOCKET_ERROR == ::shutdown(pSkContext->m_socket, SD_SEND))
				//	logm() << "shutdownʧ��, code:" << ::WSAGetLastError();
				if (0 == ::CancelIoEx((HANDLE)pSkContext->m_socket, NULL))
					logm() << "CancelIoExʧ��, code:" << ::GetLastError();
				iter++;

			}
		}
	}
	d->lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}

void IOCPServer::StartHeartbeatCheck()
{

}
