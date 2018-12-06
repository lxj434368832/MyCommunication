#include "stdafx.h"
#include "IOCPServer.h"
#include "IOCPModule.h"
#include "ConnectManage.h"
#include "ServerManage.h"
#include "ClientManage.h"
#include <WS2tcpip.h>
#include <assert.h>
#include "INetInterface.h"

IOCPServier::IOCPServier(INetInterface *pNet) :
m_pNetInterface(pNet),
m_hIOCompletionPort(INVALID_HANDLE_VALUE),
m_rscSocketContext(10000),
m_rscIoContext(1000)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;
	m_pListenSocketContext = nullptr;
}

IOCPServier::~IOCPServier()
{
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCompletionPort, 0, NULL, NULL);
	}

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::WaitForSingleObject(m_aThreadList[i], INFINITE);
		::CloseHandle(m_aThreadList[i]);
	}
	delete[] m_aThreadList;

	RELEASE_HANDLE(m_hIOCompletionPort);
	RELEASE(m_pListenSocketContext);

}

bool IOCPServier::InitIOCP(unsigned uThreadCount)
{
	assert(uThreadCount);

	m_hIOCompletionPort = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == m_hIOCompletionPort)
	{
		MLOG("������ɶ˿�ʧ�ܣ������룺%d", GetLastError());
		return false;
	}

	m_aThreadList = new HANDLE[uThreadCount];
	for (unsigned i = 0; i < uThreadCount; i++)
	{
		m_aThreadList[i] = ::CreateThread(NULL, 0, WorkerThread, this, 0, 0);
		if (NULL == m_aThreadList[i])
		{
			MLOG("����IOCP�߳�ʧ�ܣ�������:%d", GetLastError());
			return false;
		}
	}
	m_uThreadCount = uThreadCount;

	return true;
}

bool IOCPServier::StartServerListen(u_short port, unsigned iMaxServerCount)
{
	bool bRet = false;
	do
	{
		if (nullptr == m_pListenSocketContext)
		{
			m_pListenSocketContext = m_rscSocketContext.get();
		}

		if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
		{
			m_pListenSocketContext->m_socket = IOCPModule::Instance()->Socket();
			if (INVALID_SOCKET == m_pListenSocketContext->m_socket)
			{
				MLOG("����Server����socketʧ�ܣ������룺%d", WSAGetLastError());
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

		if (IOCPModule::Instance()->BindIoCompletionPort(m_pListenSocketContext, m_hIOCompletionPort)) break;

		//Ͷ�ݽ��ܲ���
		for (unsigned i = 0; i < iMaxServerCount; i++)
		{
			if (false == (bRet = PostAcceptEx(m_pListenSocketContext->m_socket)))
			{
				break;
			}
		}

	} while (0);

	if (false == bRet)
	{
		::closesocket(m_pListenSocketContext->m_socket);
		m_pListenSocketContext->m_socket = INVALID_SOCKET;
	}
	return bRet;
}

void IOCPServier::Send(int key, std::string data)
{

}

DWORD WINAPI  IOCPServier::WorkerThread(LPVOID lpParameter)
{
	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	IOCPServier *pThis = (IOCPServier*)lpParameter;
	MLOG("�߳�:%d��ʼ���С�", ::GetCurrentThreadId());

	while (true)
	{
		int iResult = 0;
		DWORD dwBytesTransfered = 0;
		LPOVERLAPPED pOverlapped = NULL;
		BOOL bSuccess = ::GetQueuedCompletionStatus(pThis->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&pSocketContext,
			&pOverlapped,
			INFINITY);

		if (FALSE == bSuccess)
		{
			iResult = ::GetLastError();
		}

		if (nullptr == pOverlapped)
		{
			if (ERROR_ABANDONED_WAIT_0 == iResult)//735L
				MLOG("��ɶ˿ھ��:%d����Ϊ�عر���!", pThis->m_hIOCompletionPort);
			else
				MLOG("��ɶ˿ڷ����˴��󣬴�����:%d ", iResult);

			MLOG(",�߳�:%d���˳���", ::GetCurrentThreadId());
			break;
		}

		if (nullptr == pSocketContext)
		{
			MLOG("�߳�:%d�����˳���", ::GetCurrentThreadId());
			break;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		pThis->HandServerOperate(iResult, pSocketContext,pIO,dwBytesTransfered);

	}

	return 0;
}

void IOCPServier::HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
	{
		PER_SOCKET_CONTEXT *pClientSkContext = nullptr;
		m_lckConnectList.lock();
		if (m_mapConnectList.find(pIO->m_socket) != m_mapConnectList.end())
		{
			pClientSkContext = m_mapConnectList[pIO->m_socket];
		}
		m_lckConnectList.unlock();

		if (0 == iResult)	//Ͷ��AcceptEx�ɹ�
		{
			SOCKADDR_IN *pClientAddr = nullptr;
			IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pClientAddr);
			char strIP[255];	inet_ntop(AF_INET, &pClientAddr->sin_addr, strIP, 255);
			MLOG("�ͻ��� %s:%d����,�û��׽��֣�%d", strIP, ntohs(pClientAddr->sin_port), pIO->m_socket);

			//֪ͨ���������
			m_pNetInterface->AddUser((unsigned)pIO->m_socket);
			pIO->m_oprateType = EOP_RECEIVE;
			UnpackReceivedData(pClientSkContext, pIO);
			if (pClientSkContext)
			{
				memcpy(&(pClientSkContext->m_clientAddr), pClientAddr, sizeof(SOCKADDR_IN));
			}
			PostAcceptEx(pSkContext->m_socket);

		}
		else
		{
			MLOG("Ͷ��AcceptExʧ�ܣ������룺%d", iResult);
			if (pClientSkContext)
			{
				m_lckConnectList.lock();
				m_mapConnectList.erase(pClientSkContext->m_socket);
				m_lckConnectList.unlock();

				int index = pClientSkContext->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
				m_aLckSocketContext[index].lock();
				RELEASE_SOCKET(pClientSkContext->m_socket);
				m_aLckSocketContext[index].unlock();

				m_rscSocketContext.put(pClientSkContext);
			}
		}
	}
	break;

	case EOP_DISCONNECT:
		DoDisconnect(iResult, pSkContext, pIO);
		break;

	case EOP_RECEIVE:
		DoReceive(iResult, pSkContext,pIO, dwBytesTransfered);
		break;

	case EOP_SEND:
		if (0 == pIO->m_overlapped.InternalHigh)
		{
			MLOG("�������һ���ѶϿ�socket��%d �����ӣ�", pIO->m_socket);
			shutdown(pIO->m_socket, SD_BOTH);
		}
		else if (iResult)
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("�������һ�������ߣ�");
			else
				MLOG("SRV����ʧ�ܣ������룺%d", iResult);

			shutdown(pIO->m_socket, SD_BOTH);
		}

		//ReleaseIOContext(pIO);

		break;
	default:
		MLOG("δ֪�ķ�������룺%d", pIO->m_oprateType);
		MAssert(false);
		break;
	}
}

//
void IOCPServier::UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	pIO->m_uDataLength += pIO->m_overlapped.InternalHigh;
	char *buf = pIO->m_szBuffer;

	PackHeader *head = NULL;
	unsigned uDataLength = 0;
	unsigned uPackLength = 0;

	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->m_uDataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_RCV_BUF_LEN)	//���յ������ݲ���ȷ
		{
			MLOG("���յ������ݳ���̫�󣬶Ͽ����ӣ�");
			PostDisconnectEx(pSkContext,pIO);
			break;
		}
		else if (uDataLength < sizeof(PackHeader) || uDataLength < uPackLength)	//���� һ����ͷ����һ������
		{
			if (buf != pIO->m_szBuffer)
			{
				memmove(pIO->m_szBuffer, buf, uDataLength);
			}
			pIO->m_wsaBuf.buf = pIO->m_szBuffer + uDataLength;
			pIO->m_wsaBuf.len = pIO->m_uBufLength - uDataLength;
			PostReceive(pSkContext, pIO);
			break;
		}
		else //������һ�����������������ʼ���
		{
			m_pNetInterface->HandSrvData(pIO->m_socket, buf, uPackLength);	//���þ����ҵ�������ݣ��а�ͷ�Ͱ���
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}

bool IOCPServier::PostAcceptEx(SOCKET listenSocket)
{
	if (INVALID_SOCKET == listenSocket)
	{
		MLOG("listenSocket��Ч��");
		return false;
	}

	PER_SOCKET_CONTEXT *pSocketContext = m_rscSocketContext.get();
	pSocketContext->Reset();

	if (INVALID_SOCKET == pSocketContext->m_socket)
	{
		if (INVALID_SOCKET == (pSocketContext->m_socket = IOCPModule::Instance()->Socket()))
		{
			MLOG("����socketʧ�ܣ������룺%d", WSAGetLastError());
			return false;
		}
		MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pSocketContext, m_hIOCompletionPort));
	}

	PER_IO_CONTEXT *pIO = &pSocketContext->m_ReceiveContext;
	pIO->m_socket = pSocketContext->m_socket;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//ʧ��
	{
		RELEASE_SOCKET(pSocketContext->m_socket);
		m_rscSocketContext.put(pSocketContext);
		return false;
	}
	else
	{
		m_lckConnectList.lock();
		m_mapConnectList[pSocketContext->m_socket] = pSocketContext;
		m_lckConnectList.unlock();
		return true;
	}
}

void IOCPServier::DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{

}

void IOCPServier::PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bHandled = false;
	int idxLock = pSkContext->m_socket % SOCKET_CONTEXT_LOCK_COUNT;

	m_aLckSocketContext[idxLock].lock();
	if (1 == pSkContext->m_iDisconnectFlag)
		bHandled = true;//�Ѿ�������Ͽ�������
	else
		pSkContext->m_iDisconnectFlag = 1;

	if (bHandled)
	{
		pIO->Reset();
		m_rscIoContext.put(pIO);
		return;
	}

	//֪ͨ�ϲ�ӿڹر�����
	m_pNetInterface->DeleteUser((unsigned)pSkContext->m_socket);
	::shutdown(pSkContext->m_socket, SD_BOTH);		//��ʱ���ͺͽ���IOӦ�û᷵�ش���
	MLOG("�ر�socket:%d�����ӡ�", pSkContext->m_socket);

	if (IOCPModule::Instance()->DisconnectEx(pIO))
	{
		//Ͷ��ʧ�ܣ�ֱ�ӻ�����Դ
		//���õķ����Ƕ�m_iDisconnectFlag�����������պͷ��Ͷ�������Ϻ��ٻ�����Դ
		m_lckConnectList.lock();
		m_mapConnectList.erase(pSkContext->m_socket);
		m_lckConnectList.unlock();

		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
		m_rscSocketContext.put(pSkContext);

		pIO->Reset();
		m_rscIoContext.put(pIO);
	}
}

void IOCPServier::DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_socket % SOCKET_CONTEXT_LOCK_COUNT;
	if (iResult)
	{
		MLOG("Server�Ͽ�����ʧ�ܣ�������:%d", iResult);
		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
	}

	m_lckConnectList.lock();
	m_mapConnectList.erase(pSkContext->m_socket);
	m_lckConnectList.unlock();
	m_rscSocketContext.put(pSkContext);

	pIO->Reset();
	m_rscIoContext.put(pIO);
}

void IOCPServier::PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (IOCPModule::Instance()->Receive(pIO))	//����ʧ�ܵĴ���
	{
		MAssert(false);
		shutdown(pIO->m_socket, SD_BOTH);
		PostDisconnectEx(pSkContext,pIO);
	}
}

void IOCPServier::DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	MLOG("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("�������һ�������ߣ�");
		else
			MLOG("SRV����ʧ�ܣ������룺%d", iResult);
		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
	else if (0 == dwBytesTransfered)	//����������ѶϿ���socket����
	{
		MLOG("�������һ���ѶϿ�socket��%d�����ӣ�", pIO->m_socket);

		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		UnpackReceivedData(pSkContext, pIO);
	}

}

void IOCPServier::PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	pIO->m_oprateType = EOP_SEND;
	if (IOCPModule::Instance()->Send(pIO))	//����ʧ�ܵĴ���
	{
		MAssert(false);
		shutdown(pIO->m_socket, SD_BOTH);	//�����Ǳ߻��յ�����
		//ReleaseIOContext(pIO);
	}
}

void IOCPServier::DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{

}

