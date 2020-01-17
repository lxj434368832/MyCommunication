#include "IOCPDef.h"
#include "IOCPClient.h"
#include "IOCPModule.h"
#include "INetInterface.h"
#include "stdafx.h"


IOCPClient::IOCPClient(INetInterface *pNet):
	IOCPBase(pNet)
{
}

IOCPClient::~IOCPClient()
{
	RELEASE(d);
}

bool IOCPClient::StartClient(unsigned uThreadCount)
{
	return InitIOCP(uThreadCount);
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
		pIO->m_wParam = iRecnnt;

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

void IOCPClient::HandConnectFailed(PER_SOCKET_CONTEXT *pSkContext)
{
	d->lckConnectList.lock();
	d->mapConnectList.erase(pSkContext->m_uUserKey);
	d->lckConnectList.unlock();

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		LOGM("��⵽���ӶϿ���ʶ����ֱ�ӻ�����Դ��");
		//ֱ�ӻ�����Դ
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return;
	}

	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	if ((int)pIO->m_wParam <= 0) //��ʾ����Ҫ������ֱ�ӻ�����Դ
	{
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, false);

		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
	}
	else //	��ʾ��Ҫ����������������б���
	{
		d->lckStayConnect.lock();
		d->mapStayConnect[pSkContext->m_uUserKey] = pSkContext;
		d->lckStayConnect.unlock();
	}
}

void IOCPClient::HeartbeatHandle()
{
	d->hHeartbeatEvent = ::CreateEvent(NULL, false, false, NULL);
	if (NULL == d->hHeartbeatEvent)
	{
		loge() << "���������¼�ʧ�ܣ�";
		return;
	}
	
	std::string strMsg = "�������ݡ�";

	while (true)
	{
		DWORD dwRet = ::WaitForSingleObject(d->hHeartbeatEvent, 5000);
		if (WAIT_OBJECT_0 == dwRet)
		{
			logm() << "�����˳������̡߳�";
			break;
		}
		else if (WAIT_FAILED == dwRet)
		{
			loge() << "�����̷߳�������" << ::GetLastError();
		}

		//����������б�
		d->lckStayConnect.lock();
		for (auto iter = d->mapStayConnect.begin(); iter != d->mapStayConnect.end(); )
		{
			if (false == PostConnectEx(iter->second))
			{
				d->pNetInterface->ConnectNotify(iter->first, false);
				iter = d->mapStayConnect.erase(iter);
			}
			else
				iter++;
		}
		d->lckStayConnect.unlock();

		d->lckConnectList.lock();
		for (auto iter = d->mapConnectList.begin(); iter != d->mapConnectList.end(); iter++)
		{
			Send(iter->first, 0, strMsg.data(), strMsg.size());
		}
		d->lckConnectList.unlock();
	}
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
				pSkContext->m_iDisconnectFlag = 1;
			//else
			//	m_pNetInterface->DeleteUser(pSkContext->m_uUserKey);

			::shutdown(pSkContext->m_socket, SD_BOTH);
			//RELEASE_SOCKET(pSkContext->m_socket);
		}
		else
		{
			LOGM("�����б��е���Դ����ȷ,��Ӧ�ô���INVALID_SOCKET��");
		}

		//m_mapConnectList.erase(iter++);
		//m_rscSocketContext.put(pSkContext);
	}
	d->lckConnectList.unlock();

	IOCPBase::UninitIOCP();
}
