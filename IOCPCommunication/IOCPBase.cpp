#include "IOCPBase.h"
#include "IOCPModule.h"
#include "INetInterface.h"
#include "stdafx.h"
#include <assert.h>

IOCPBase::IOCPBase(INetInterface *pNet):
	m_pNetInterface(pNet),
	m_hIOCompletionPort(INVALID_HANDLE_VALUE),
	m_rscSocketContext(SOCKET_RESOURCE_COUNT),
	m_rscIoContext(IO_RESOURCE_COUNT)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;
	m_pListenSocketContext = nullptr;
	if (m_pNetInterface)
	{
		m_pNetInterface->SetNetworkCallback(std::bind(&IOCPBase::Send, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			std::bind(&IOCPBase::Disconnect, this, std::placeholders::_1));
	}
}

IOCPBase::~IOCPBase()
{
	m_pNetInterface = nullptr;
}

bool IOCPBase::InitIOCP(unsigned uThreadCount)
{
	m_hIOCompletionPort = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == m_hIOCompletionPort)
	{
		MLOG("������ɶ˿�ʧ�ܣ������룺%d", GetLastError());
		return false;
	}

	if (0 == uThreadCount)
		m_uThreadCount = IOCPModule::Instance()->GetProcessorCount() * 2 + 1;
	else
		m_uThreadCount = uThreadCount;

	m_aThreadList = new HANDLE[m_uThreadCount];
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		m_aThreadList[i] = ::CreateThread(NULL, 0, WorkerThread, this, 0, 0);
		if (NULL == m_aThreadList[i])
		{
			MLOG("����IOCP�߳�ʧ�ܣ�������:%d", GetLastError());
			return false;
		}
	}

	return true;
}

void IOCPBase::UninitIOCP()
{
	//���ŵعر��߳�
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCompletionPort, 0, NULL, NULL);
	}

	::WaitForMultipleObjects(m_uThreadCount, m_aThreadList, TRUE, 2000);

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::CloseHandle(m_aThreadList[i]);
	}
	delete[] m_aThreadList;
	m_aThreadList = nullptr;

	//�ر���ɶ˿�
	RELEASE_HANDLE(m_hIOCompletionPort);
}

bool IOCPBase::StartServerListen(u_short port, unsigned iMaxConnectCount)
{
	bool bRet = false;
	/*do
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
		m_rscSocketContext.put(m_pListenSocketContext);
		m_pListenSocketContext = nullptr;
	}*/
	return bRet;
}

bool IOCPBase::AddConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt)
{
	bool bRet = false;
	/*PER_SOCKET_CONTEXT *pSkContext = m_rscSocketContext.get();
	do
	{
		PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
		pIO->m_wParam = iRecnnt;

		if (INVALID_SOCKET == pSkContext->m_socket)
		{
			if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
			{
				MLOG("����socketʧ�ܣ������룺%d", WSAGetLastError());
				break;
			}
		}

		if (IOCPModule::Instance()->BindIoCompletionPort(pSkContext, m_hIOCompletionPort)) break;
		pIO->m_socket = pSkContext->m_socket;

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		if(IOCPModule::Instance()->Bind(pIO->m_socket, (LPSOCKADDR)&addrLocal)) break;

		SOCKADDR_IN *pSrvAddr = &pSkContext->m_clientAddr;
		pSrvAddr->sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &pSrvAddr->sin_addr);
		pSrvAddr->sin_port = htons(port);

		bRet = PostConnectEx(pIO, (LPSOCKADDR)pSrvAddr);

	} while (0);

	if (bRet)
	{
		m_lckConnectList.lock();
		m_mapConnectList[pSkContext->m_socket] = pSkContext;
		m_lckConnectList.unlock();
	}
	else
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		m_rscSocketContext.put(pSkContext);
	}*/
	return bRet;
}

DWORD WINAPI IOCPBase::WorkerThread(LPVOID lpParameter)
{

	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	IOCPBase *pThis = (IOCPBase*)lpParameter;
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
			if (WAIT_TIMEOUT == iResult)
			{
				::Sleep(1000);
				continue;
			}
			else if (0 == iResult)
			{
				logm() << "ThreadID:" << ::GetCurrentThreadId() << " �����˳���";
			}
			else if (ERROR_ABANDONED_WAIT_0 == iResult)//735L
				MLOG("��ɶ˿ھ��:%d����Ϊ�عر���!", pThis->m_hIOCompletionPort);
			else
				MLOG("��ɶ˿ڷ����˴��󣬴�����:%d ", iResult);

			break;
		}

		if (nullptr == pSocketContext)
		{
			MLOG("�߳�:%d�����˳���", ::GetCurrentThreadId());
			break;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		pThis->HandServerOperate(iResult, pSocketContext, pIO, dwBytesTransfered);

	}

	return 0;
}

void IOCPBase::HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
		DoAccept(iResult, pSkContext, pIO);
		break;
	case EOP_CONNECT:
		DoConnect(iResult, pSkContext, pIO);
		break;

	case EOP_DISCONNECT:
		DoDisconnect(iResult, pSkContext, pIO);
		break;

	case EOP_RECEIVE:
		DoReceive(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	case EOP_SEND:
		DoSend(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	default:
		MLOG("δ֪�ķ�������룺%d", pIO->m_oprateType);
		MAssert(false);
		break;
	}
}

bool IOCPBase::PostConnectEx(PER_SOCKET_CONTEXT *pSkContext)
{
	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	pIO->m_oprateType = EOP_CONNECT;
	if (IOCPModule::Instance()->ConnectEx(pIO, (LPSOCKADDR)&pSkContext->m_clientAddr))
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		m_rscSocketContext.put(pSkContext);
		return false;
	}
	else
	{
		m_lckConnectList.lock();
		m_mapConnectList[pSkContext->m_uUserKey] = pSkContext;
		m_lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::DoConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (0 == iResult)
	{
		//֪ͨ���������
		m_pNetInterface->AddUser((unsigned)pSkContext->m_uUserKey);
		//Ͷ������
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pSkContext, pIO);
	}
	else
	{
		if (ERROR_OPERATION_ABORTED == iResult || WSAENOTSOCK == iResult)
		{
			PER_IO_CONTEXT *pIONew = m_rscIoContext.get();
			pIONew->m_socket = pSkContext->m_socket;
			PostDisconnectEx(pSkContext, pIONew);
		}
		else if (ERROR_CONNECTION_REFUSED == iResult)
		{
			logm() << "����˻�δ�������������ԡ�";
			PostConnectEx(pSkContext);
		}
		else
		{
			MLOG("socket:%dͶ��ConnectExʧ�ܣ������룺%d��", pIO->m_socket, iResult);
			PostConnectEx(pSkContext);
		}
	}
}

bool IOCPBase::PostAcceptEx(SOCKET listenSocket)
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
	pSocketContext->m_uUserKey = pSocketContext->m_socket;

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
		m_mapConnectList[pSocketContext->m_uUserKey] = pSocketContext;
		m_lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	PER_SOCKET_CONTEXT *pClientSkContext = nullptr;
	m_lckConnectList.lock();
	if (m_mapConnectList.find(pIO->m_socket) != m_mapConnectList.end())	//����˵�userkey����socket
	{
		pClientSkContext = m_mapConnectList[pIO->m_socket];
	}
	m_lckConnectList.unlock();

	if (0 == iResult)	//Ͷ��AcceptEx�ɹ�
	{
		SOCKADDR_IN *pClientAddr = nullptr;
		IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pClientAddr);
		;
		MLOG("�ͻ���%s:%d����,�û��׽��֣�%d", IOCPModule::Instance()->GetIPAddress(&pClientAddr->sin_addr).c_str(), 
			ntohs(pClientAddr->sin_port), pIO->m_socket);

		if (pClientSkContext)
		{
			memcpy(&(pClientSkContext->m_clientAddr), pClientAddr, sizeof(SOCKADDR_IN));
		}
		else
		{
			MLOG("pClientSkContextΪ�գ���������");
			assert(pClientSkContext);
		}
		//֪ͨ���������
		m_pNetInterface->AddUser(pClientSkContext->m_uUserKey);
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pClientSkContext, pIO);
		PostAcceptEx(pSkContext->m_socket);

	}
	else
	{
		MLOG("Ͷ��AcceptExʧ�ܣ������룺%d", iResult);
		if (pClientSkContext)
		{
			m_lckConnectList.lock();
			m_mapConnectList.erase(pClientSkContext->m_uUserKey);
			m_lckConnectList.unlock();

			int idxLock = pClientSkContext->m_uUserKey % SOCKET_CONTEXT_LOCK_COUNT;
			m_aLckSocketContext[idxLock].lock();
			RELEASE_SOCKET(pClientSkContext->m_socket);
			m_aLckSocketContext[idxLock].unlock();

			m_rscSocketContext.put(pClientSkContext);
		}
	}
}

void IOCPBase::Send(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	if (nullptr == data || 0 == uLength)
	{
		MLOG("���͵�����Ϊ�գ�");
		return;
	}

	if (uLength > MAX_RCV_BUF_LEN - sizeof(PackHeader))
	{
		MLOG("��Ҫ��������(%d)���ڷ��ͻ�����(%d),���ܷ������ݣ�", uLength, MAX_RCV_BUF_LEN - sizeof(PackHeader));
		return;
	}

	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	int idxLock = uUserKey % SOCKET_CONTEXT_LOCK_COUNT;

	m_lckConnectList.lock();
	if (m_mapConnectList.find(uUserKey) != m_mapConnectList.end())
	{
		pSkContext = m_mapConnectList.at(uUserKey);
	}
	m_lckConnectList.unlock();

	if (nullptr == pSkContext)
	{
		MLOG("��������ʧ�ܣ������ڵ�ǰ�û�:%d", uUserKey);
		return;
	}

	m_aLckSocketContext[idxLock].lock();
	PackSendData(pSkContext, uMsgType, data, uLength);
	m_aLckSocketContext[idxLock].unlock();
	return;
}

void IOCPBase::PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length)
{
	unsigned uLeftLen = length;
	unsigned uCurrentCpyLen = 0;

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		MLOG("��ǰ�û�:%d�ѶϿ���ֹͣ��������", pSkContext->m_socket);
		return;
	}

	for (int i = 0; 0 != uLeftLen; i++)
	{
		if (uLeftLen < 0)
		{
			MLOG("���������߼����ִ���uLeftLenС��0��");
			break;
		}

		PER_IO_CONTEXT *pIO = m_rscIoContext.get();
		pIO->m_socket = pSkContext->m_socket;
		if (0 == i)
		{
			PackHeader head;
			unsigned uHeadSize = sizeof(PackHeader);
			head.uMsgType = uMsgType;
			head.ulBodyLength = length;
			memcpy(pIO->m_szBuffer, (char*)&head, uHeadSize);
			uCurrentCpyLen = min(uLeftLen, MAX_BUF_LEN - uHeadSize);
			memcpy(pIO->m_szBuffer + uHeadSize, data, uCurrentCpyLen);
			pIO->m_uDataLength = uCurrentCpyLen + uHeadSize;

		}
		else
		{
			uCurrentCpyLen = min(uLeftLen, MAX_BUF_LEN);
			memcpy(pIO->m_szBuffer, data, uCurrentCpyLen);
			pIO->m_uDataLength = uCurrentCpyLen;
		}

		pIO->m_wsaBuf.buf = pIO->m_szBuffer;
		pIO->m_wsaBuf.len = pIO->m_uDataLength;
		if (false == PostSend(pSkContext, pIO))
		{
			MLOG("��������ʧ�ܣ���ʣ%d byte����δ���ͣ�", uLeftLen);
			break;
		}

		uLeftLen -= uCurrentCpyLen;
		data += uCurrentCpyLen;
	}

}

bool IOCPBase::PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bRet = true;
	if (1 == pSkContext->m_iDisconnectFlag)
	{
		MLOG("�û��Ѿ��Ͽ�����:%d,��ֹͣ�������ݣ�", pIO->m_socket);
		bRet = false;
	}
	else if (nullptr == pIO)	//��������IO
	{
		if (pSkContext->m_queueIoContext.empty())
		{
			pSkContext->m_iSendPendingFlag = 0;
		}
		else
		{
			pIO = pSkContext->m_queueIoContext.front();
			pSkContext->m_queueIoContext.pop();
			if (IOCPModule::Instance()->Send(pIO))	//����ʧ�ܵĴ���
			{
				PostDisconnectEx(pSkContext, pIO);
				bRet = false;
			}
		}
	}
	else
	{
		if (pSkContext->m_queueIoContext.empty())
		{
			//Ͷ�ݵ�һ������
			pSkContext->m_iSendPendingFlag = 1;
			if (IOCPModule::Instance()->Send(pIO))	//����ʧ�ܵĴ���
			{
				PostDisconnectEx(pSkContext, pIO);
				bRet = false;
			}
		}
		else
		{
			//����ǰIO���뷢�Ͷ���
			pSkContext->m_queueIoContext.push(pIO);
		}
	}
	return bRet;
}

void IOCPBase::DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	MLOG("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("�������һ�������ߣ�");
		else
			MLOG("��������ʧ�ܣ������룺%d", iResult);

		PostDisconnectEx(pSkContext, pIO);
	}
	else if (0 == dwBytesTransfered)	//����������ѶϿ���socket����
	{
		MLOG("�������һ���ѶϿ�socket��%d�����ӣ�", pIO->m_socket);

		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		int idxLock = pSkContext->m_uUserKey % SOCKET_CONTEXT_LOCK_COUNT;
		pIO->Reset();
		m_rscIoContext.put(pIO);
		m_aLckSocketContext[idxLock].lock();
		PostSend(pSkContext, nullptr);
		m_aLckSocketContext[idxLock].unlock();

	}
}

void IOCPBase::Disconnect(UserKey uUserKey)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	PER_IO_CONTEXT *pIO = nullptr;

	m_lckConnectList.lock();
	if (m_mapConnectList.end() != m_mapConnectList.find(uUserKey))
	{
		pSkContext = m_mapConnectList.at(uUserKey);
		pIO = m_rscIoContext.get();
	}
	m_lckConnectList.unlock();

	if (pSkContext && pIO)
	{
		PostDisconnectEx(pSkContext, pIO);
	}
}

void IOCPBase::PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bHandled = false;
	int idxLock = pSkContext->m_uUserKey % SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&m_aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			//�Ѿ�������Ͽ�������,����IO��Դ
			pIO->Reset();
			m_rscIoContext.put(pIO);
			return;
		}
		else
		{
			//����Ͽ�����
			pSkContext->m_iDisconnectFlag = 1;
			//�����Ͷ���
			while (false == pSkContext->m_queueIoContext.empty())
			{
				PER_IO_CONTEXT *pIOTemp = pSkContext->m_queueIoContext.front();
				m_rscIoContext.put(pIOTemp);
				pSkContext->m_queueIoContext.pop();
			}
		}
	}

	//֪ͨ�ϲ�ӿڹر�����
	m_pNetInterface->DeleteUser(pSkContext->m_uUserKey);
	::shutdown(pSkContext->m_socket, SD_BOTH);		//��ʱ���ͺͽ���IOӦ�û᷵�ش���
	MLOG("�ر�user:%d�����ӡ�", pSkContext->m_uUserKey);

	//�������б������
	m_lckConnectList.lock();
	m_mapConnectList.erase(pSkContext->m_uUserKey);
	m_lckConnectList.unlock();

	if (IOCPModule::Instance()->DisconnectEx(pIO))
	{
		//Ͷ��ʧ�ܣ�ֱ�ӻ�����Դ
		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
		m_rscSocketContext.put(pSkContext);

		pIO->Reset();
		m_rscIoContext.put(pIO);
	}
}

void IOCPBase::DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % SOCKET_CONTEXT_LOCK_COUNT;
	if (iResult)
	{
		MLOG("Server�Ͽ�����ʧ�ܣ�������:%d", iResult);
		m_aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		m_aLckSocketContext[idxLock].unlock();
	}

	m_rscSocketContext.put(pSkContext);

	pIO->Reset();
	m_rscIoContext.put(pIO);
}

void IOCPBase::PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&m_aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			MLOG("��⵽���ӶϿ�����ֹͣ�������ݣ�");
			return;
		}
	}

	if (IOCPModule::Instance()->Receive(pIO))	//����ʧ�ܵĴ���
	{
		PER_IO_CONTEXT *pIOTemp = m_rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
}

void IOCPBase::DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
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

void IOCPBase::UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	PackHeader *head = NULL;
	char *buf = pIO->m_szBuffer;

	unsigned uDataLength = 0;	//ʣ�����ݵĳ���
	unsigned uPackLength = 0;	//��������ݵĳ���

	pIO->m_uDataLength += pIO->m_overlapped.InternalHigh;
	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->m_uDataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_RCV_BUF_LEN)	//���յ������ݲ���ȷ
		{
			MLOG("���յ������ݳ���̫�󣬶Ͽ����ӣ�");
			PostDisconnectEx(pSkContext, pIO);
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
			if (CHECK_VALUE != head->uCheck)
			{
				MLOG("��У�鲻�ԣ����Ͽ����ӣ�");
				PostDisconnectEx(pSkContext, pIO);
				break;
			}
			m_pNetInterface->HandData(pSkContext->m_uUserKey, head->uMsgType, buf + sizeof(PackHeader), head->ulBodyLength);	//���þ����ҵ�������ݣ��а�ͷ�Ͱ���
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}
