#include "..\include\IOCPDef.h"
#include "..\include\IOCPClient.h"
#include "..\include\IOCPModule.h"
#include "..\include\INetInterface.h"
#include "..\include\stdafx.h"


IOCPClient::IOCPClient(INetInterface *pNet):
	IOCPBase(pNet)
{
}

IOCPClient::~IOCPClient()
{
}

bool IOCPClient::StartClient(unsigned uThreadCount)
{
	if (false == InitIOCP(uThreadCount)) return false;
	return true;
}

bool IOCPClient::AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();
	pSkContext->m_uUserKey = uUserKey;

	do
	{
		PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
		pIO->m_lParam = iRecnnt;

		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				LOGM("����socketʧ�ܣ������룺%d", WSAGetLastError());
				break;
			}
		}

		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort)) break;
		pIO->m_socket = pSkContext->m_socket;

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if (IOCPModule::Instance()->Bind(pIO->m_socket, (LPSOCKADDR)&addrLocal)) break;

		SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
		pSrvAddr->sin_family = AF_INET;
		IOCPModule::Instance()->ParseIPAddress(ip, &pSrvAddr->sin_addr);
		pSrvAddr->sin_port = htons(port);

		bRet = PostConnectEx(pSkContext);

	} while (0);

	return bRet;
}

void IOCPClient::StopClient()
{
	//�ر�����socket�����������й�����ص�IO����
	logm() << "�ر�����socket���ӡ�";
	d->lckConnectList.lock();
	for (auto iter = d->mapConnectList.begin(); iter != d->mapConnectList.end(); iter++)
	{
		PER_SOCKET_CONTEXT *pSkContext = iter->second;
		if (INVALID_SOCKET != pSkContext->m_socket)
		{
			if (EOP_CONNECT == pSkContext->m_ReceiveContext.m_oprateType)
			{
				if (0 == ::CancelIoEx((HANDLE)pSkContext->m_socket, NULL))
					logm() << "CancelIoExʧ��, code:" << ::GetLastError();
				//::InterlockedExchange(&pSkContext->m_iDisconnectFlag, 1);
			}
			else
			{
				if (0 == ::CancelIoEx((HANDLE)pSkContext->m_socket, NULL))
					logm() << "CancelIoExʧ��, code:" << ::GetLastError();
			}
		}
		else
		{
			LOGM("�����б��е���Դ����ȷ,��Ӧ�ô���INVALID_SOCKET��");
		}
	}
	d->lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}
