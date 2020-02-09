#include "..\include\IOCPDef.h"
#include "..\include\IOCPBase.h"
#include "..\include\IOCPModule.h"
#include "..\include\stdafx.h"
#include <assert.h>

IOCPBase::IOCPBase(INetInterface *pNet)
:d(new IOCPBaseData(pNet))
{
	if (d->pNetInterface)
	{
		d->pNetInterface->SetNetworkCallback(std::bind(&IOCPBase::Send, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),
			std::bind(&IOCPBase::Disconnect, this, std::placeholders::_1));
	}
}

IOCPBase::~IOCPBase()
{
	RELEASE(d);
}

bool IOCPBase::InitIOCP(unsigned uThreadCount)
{
	d->hIOCompletionPort = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == d->hIOCompletionPort)
	{
		LOGM("������ɶ˿�ʧ�ܣ������룺%d", GetLastError());
		return false;
	}

	if (0 == uThreadCount)
		d->uThreadCount = IOCPModule::Instance()->GetProcessorCount() * 2 + 1;
	else
		d->uThreadCount = uThreadCount;

	d->aThreadList = new std::thread[d->uThreadCount];
	for (unsigned i = 0; i < d->uThreadCount; i++)
	{
		d->aThreadList[i] = std::move(std::thread(&IOCPBase::WorkThread, this));
	}

	return true;
}

void IOCPBase::UninitIOCP()
{
	logm() << "�ر���ɶ˿��̶߳��С�";
	//���ŵعر��߳�
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

	logm() << "�ر���ɶ˿ڡ�";
	//�ر���ɶ˿�
	RELEASE_HANDLE(d->hIOCompletionPort);
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
				LOGM("����socketʧ�ܣ������룺%d", WSAGetLastError());
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

void IOCPBase::WorkThread()
{
	PER_IO_CONTEXT *pIO = nullptr;
	PER_SOCKET_CONTEXT *pSocketContext = nullptr;
	LOGM("�߳�:%d��ʼ���С�", std::this_thread::get_id());

	while (true)
	{
		int iResult = 0;
		DWORD dwBytesTransfered = 0;
		LPOVERLAPPED pOverlapped = NULL;
		BOOL bSuccess = ::GetQueuedCompletionStatus(d->hIOCompletionPort,
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
				LOGM("��ɶ˿ھ��:%d����Ϊ�عر���!", d->hIOCompletionPort);
			else
				LOGM("��ɶ˿ڷ����˴��󣬴�����:%d ", iResult);

			break;
		}

		if (nullptr == pSocketContext)
		{
			LOGM("�߳�:%d�����˳���", ::GetCurrentThreadId());
			break;
		}

		pIO = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_overlapped);
		HandServerOperate(iResult, pSocketContext, pIO, dwBytesTransfered);

	}

}

void IOCPBase::HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	switch (pIO->m_oprateType)
	{
	case EOP_ACCEPT:
		HandAccept(iResult, pSkContext, pIO);
		break;
	case EOP_CONNECT:
		HandConnect(iResult, pSkContext, pIO);
		break;

	case EOP_DISCONNECT:
		HandDisconnect(iResult, pSkContext, pIO);
		break;

	case EOP_RECEIVE:
		HandReceive(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	case EOP_SEND:
		HandSend(iResult, pSkContext, pIO, dwBytesTransfered);
		break;

	default:
		LOGM("δ֪�ķ�������룺%d", pIO->m_oprateType);
		assert(false);
		break;
	}
}

bool IOCPBase::PostConnectEx(PER_SOCKET_CONTEXT *pSkContext)
{
	d->lckConnectList.lock();
	d->mapConnectList[pSkContext->m_uUserKey] = pSkContext;
	d->lckConnectList.unlock();

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		LOGM("��⵽�����Ѿ��Ͽ�����ֱ�ӻ�����Դ��");
		d->lckConnectList.lock();
		d->mapConnectList.erase(pSkContext->m_uUserKey);
		d->lckConnectList.unlock();
		//ֱ�ӻ�����Դ
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return false;
	}

	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	pIO->m_oprateType = EOP_CONNECT;
	if (IOCPModule::Instance()->ConnectEx(pIO, (LPSOCKADDR)&pSkContext->m_clientAddr))
	{
		d->lckConnectList.lock();
		d->mapConnectList.erase(pSkContext->m_uUserKey);
		d->lckConnectList.unlock();
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return false;
	}
	else
	{
		d->lckConnectList.lock();
		d->mapConnectList[pSkContext->m_uUserKey] = pSkContext;
		d->lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::HandConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	if (0 == iResult)
	{
		//֪ͨ�ϲ����ӳɹ�
		d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, true);
		//Ͷ�ݽ���
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pSkContext, pIO);
	}
	else
	{
		if (ERROR_OPERATION_ABORTED == iResult || WSAENOTSOCK == iResult)
		{
			logm() << "�û��Ѿ�ȡ�����ӣ���ֱ�ӻ�����Դ��";

			d->lckConnectList.lock();
			d->mapConnectList.erase(pSkContext->m_uUserKey);
			d->lckConnectList.unlock();
			//ֱ�ӻ�����Դ
			RELEASE_SOCKET(pSkContext->m_socket);
			d->rscSocketContext.put(pSkContext);
		}
		else if (ERROR_CONNECTION_REFUSED == iResult)
		{
			logm() << "����˻�δ�������������ԡ�";
			HandConnectFailed(pSkContext);
		}
		else
		{
			LOGM("socket:%dͶ��ConnectExʧ�ܣ������룺%d��", pIO->m_socket, iResult);
			HandConnectFailed(pSkContext);
		}
	}
}

void IOCPBase::HandConnectFailed(PER_SOCKET_CONTEXT *pSkContext)
{
	d->lckConnectList.lock();
	d->mapConnectList.erase(pSkContext->m_uUserKey);
	d->lckConnectList.unlock();
	RELEASE_SOCKET(pSkContext->m_socket);

	//֪ͨ�ϲ�����ʧ��
	d->pNetInterface->ConnectNotify(pSkContext->m_uUserKey, false);
	d->rscSocketContext.put(pSkContext);
}

bool IOCPBase::PostAcceptEx(SOCKET listenSocket)
{
	if (INVALID_SOCKET == listenSocket)
	{
		LOGM("listenSocket��Ч��");
		return false;
	}

	PER_SOCKET_CONTEXT *pSkContext = d->rscSocketContext.get();
	pSkContext->Reset();

	if (INVALID_SOCKET == pSkContext->m_socket)
	{
		if (INVALID_SOCKET == (pSkContext->m_socket = IOCPModule::Instance()->Socket()))
		{
			LOGM("����socketʧ�ܣ������룺%d", WSAGetLastError());
			return false;
		}
		assert(0 == IOCPModule::Instance()->BindIoCompletionPort(pSkContext, d->hIOCompletionPort));
	}
	pSkContext->m_uUserKey = pSkContext->m_socket;

	PER_IO_CONTEXT *pIO = &pSkContext->m_ReceiveContext;
	pIO->m_socket = pSkContext->m_socket;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//ʧ��
	{
		RELEASE_SOCKET(pSkContext->m_socket);
		d->rscSocketContext.put(pSkContext);
		return false;
	}
	else
	{
		d->lckConnectList.lock();
		d->mapConnectList[pSkContext->m_uUserKey] = pSkContext;
		d->lckConnectList.unlock();
		return true;
	}
}

void IOCPBase::HandAccept(int iResult, PER_SOCKET_CONTEXT *pListenSkContext, PER_IO_CONTEXT* pIO)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	d->lckConnectList.lock();
	if (d->mapConnectList.find(pIO->m_socket) != d->mapConnectList.end())	//����˵�userkey����socket
	{
		pSkContext = d->mapConnectList[pIO->m_socket];
	}
	d->lckConnectList.unlock();

	if (0 == iResult)	//Ͷ��AcceptEx�ɹ�
	{
		SOCKADDR_IN *pClientAddr = nullptr;
		IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pClientAddr);
		;
		LOGM("�ͻ���%s:%d����,�û��׽��֣�%d", IOCPModule::Instance()->GetIPAddress(&pClientAddr->sin_addr).c_str(), 
			ntohs(pClientAddr->sin_port), pIO->m_socket);

		if (pSkContext)
		{
			memcpy(&(pSkContext->m_clientAddr), pClientAddr, sizeof(SOCKADDR_IN));
		}
		else
		{
			LOGM("pClientSkContextΪ�գ���������");
			assert(pSkContext);
		}

		//֪ͨ����������ӵ���
		d->pNetInterface->AddUser(pSkContext->m_uUserKey);
		pIO->m_wsaBuf.len = pIO->m_uBufLength;
		PostReceive(pSkContext, pIO);
		PostAcceptEx(pListenSkContext->m_socket);
	}
	else
	{
		LOGM("Ͷ��AcceptExʧ�ܣ������룺%d", iResult);
		if (pSkContext)
		{
			d->lckConnectList.lock();
			d->mapConnectList.erase(pSkContext->m_uUserKey);
			d->lckConnectList.unlock();

			int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
			d->aLckSocketContext[idxLock].lock();
			RELEASE_SOCKET(pSkContext->m_socket);
			d->aLckSocketContext[idxLock].unlock();

			d->rscSocketContext.put(pSkContext);
		}
	}
}

void IOCPBase::Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength)
{
	if (nullptr == data || 0 == uLength)
	{
		LOGM("���͵�����Ϊ�գ�");
		return;
	}

	if (uLength > MAX_RCV_BUF_LEN - sizeof(PackHeader))
	{
		LOGM("��Ҫ��������(%d)���ڷ��ͻ�����(%d),���ܷ������ݣ�", uLength, MAX_RCV_BUF_LEN - sizeof(PackHeader));
		return;
	}

	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	int idxLock = uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;

	d->lckConnectList.lock();
	if (d->mapConnectList.find(uUserKey) != d->mapConnectList.end())
	{
		pSkContext = d->mapConnectList.at(uUserKey);
	}
	d->lckConnectList.unlock();

	if (nullptr == pSkContext)
	{
		LOGM("��������ʧ�ܣ������ڵ�ǰ�û�:%d", uUserKey);
		return;
	}

	d->aLckSocketContext[idxLock].lock();
	PackSendData(pSkContext, uMsgType, data, uLength);
	d->aLckSocketContext[idxLock].unlock();
	return;
}

void IOCPBase::PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length)
{
	unsigned uLeftLen = length;
	unsigned uCurrentCpyLen = 0;

	if (1 == pSkContext->m_iDisconnectFlag)
	{
		LOGM("��ǰ�û�:%d�ѶϿ���ֹͣ��������", pSkContext->m_socket);
		return;
	}

	for (int i = 0; 0 != uLeftLen; i++)
	{
		if (uLeftLen < 0)
		{
			LOGM("���������߼����ִ���uLeftLenС��0��");
			break;
		}

		PER_IO_CONTEXT *pIO = d->rscIoContext.get();
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
			LOGM("��������ʧ�ܣ���ʣ%d byte����δ���ͣ�", uLeftLen);
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
		LOGM("�û��Ѿ��Ͽ�����:%d,��ֹͣ�������ݣ�", pIO->m_socket);
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

void IOCPBase::HandSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	LOGM("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			LOGM("�������һ�������ߣ�");
		else
			LOGM("��������ʧ�ܣ������룺%d", iResult);

		PostDisconnectEx(pSkContext, pIO);
	}
	else if (0 == dwBytesTransfered)	//����������ѶϿ���socket����
	{
		LOGM("�������һ���ѶϿ�socket��%d�����ӣ�", pIO->m_socket);

		PostDisconnectEx(pSkContext, pIO);
	}
	else
	{
		int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
		pIO->Reset();
		d->rscIoContext.put(pIO);
		d->aLckSocketContext[idxLock].lock();
		PostSend(pSkContext, nullptr);
		d->aLckSocketContext[idxLock].unlock();

	}
}

void IOCPBase::Disconnect(unsigned uUserKey)
{
	PER_SOCKET_CONTEXT *pSkContext = nullptr;
	PER_IO_CONTEXT *pIO = nullptr;

	d->lckConnectList.lock();
	if (d->mapConnectList.end() != d->mapConnectList.find(uUserKey))
	{
		pSkContext = d->mapConnectList.at(uUserKey);
		pIO = d->rscIoContext.get();
	}
	d->lckConnectList.unlock();

	if (pSkContext && pIO)
	{
		PostDisconnectEx(pSkContext, pIO);
	}
}

void IOCPBase::PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	bool bHandled = false;
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&d->aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			//�Ѿ�������Ͽ�������,����IO��Դ
			pIO->Reset();
			d->rscIoContext.put(pIO);
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
				d->rscIoContext.put(pIOTemp);
				pSkContext->m_queueIoContext.pop();
			}
		}
	}

	//֪ͨ�ϲ�ӿڹر�����
	d->pNetInterface->DeleteUser(pSkContext->m_uUserKey);
	::shutdown(pSkContext->m_socket, SD_BOTH);		//��ʱ���ͺͽ���IOӦ�û᷵�ش���
	LOGM("�ر�user:%d�����ӡ�", pSkContext->m_uUserKey);

	//�������б������
	d->lckConnectList.lock();
	d->mapConnectList.erase(pSkContext->m_uUserKey);
	d->lckConnectList.unlock();

	if (IOCPModule::Instance()->DisconnectEx(pIO))
	{
		//Ͷ��ʧ�ܣ�ֱ�ӻ�����Դ
		d->aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		d->aLckSocketContext[idxLock].unlock();
		d->rscSocketContext.put(pSkContext);

		pIO->Reset();
		d->rscIoContext.put(pIO);
	}
}

void IOCPBase::HandDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
	if (iResult)
	{
		LOGM("�Ͽ�����ʧ�ܣ�������:%d", iResult);
		d->aLckSocketContext[idxLock].lock();
		RELEASE_SOCKET(pSkContext->m_socket);
		d->aLckSocketContext[idxLock].unlock();
	}

	d->rscSocketContext.put(pSkContext);

	pIO->Reset();
	d->rscIoContext.put(pIO);
}

void IOCPBase::PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO)
{
	int idxLock = pSkContext->m_uUserKey % IOCPBaseData::SOCKET_CONTEXT_LOCK_COUNT;
	{
		MAutoLock lck(&d->aLckSocketContext[idxLock]);
		if (1 == pSkContext->m_iDisconnectFlag)
		{
			LOGM("��⵽���ӶϿ�����ֹͣ�������ݣ�");
			return;
		}
	}

	if (IOCPModule::Instance()->Receive(pIO))	//����ʧ�ܵĴ���
	{
		PER_IO_CONTEXT *pIOTemp = d->rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
}

void IOCPBase::HandReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered)
{
	LOGM("dwBytesTransfered:%d  InternalHigh:%d.", dwBytesTransfered, pIO->m_overlapped.InternalHigh);

	if (iResult)
	{
		if (ERROR_NETNAME_DELETED == iResult)
			LOGM("�������һ�������ߣ�");
		else
			LOGM("SRV����ʧ�ܣ������룺%d", iResult);

		PER_IO_CONTEXT *pIOTemp = d->rscIoContext.get();
		pIOTemp->m_socket = pSkContext->m_socket;
		PostDisconnectEx(pSkContext, pIOTemp);
	}
	else if (0 == dwBytesTransfered)	//����������ѶϿ���socket����
	{
		LOGM("�������һ���ѶϿ�socket��%d�����ӣ�", pIO->m_socket);

		PER_IO_CONTEXT *pIOTemp = d->rscIoContext.get();
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
			LOGM("���յ������ݳ���̫�󣬶Ͽ����ӣ�");
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
				LOGM("��У�鲻�ԣ����Ͽ����ӣ�");
				PostDisconnectEx(pSkContext, pIO);
				break;
			}
			d->pNetInterface->HandData(pSkContext->m_uUserKey, head->uMsgType, buf + sizeof(PackHeader), head->ulBodyLength);	//���þ����ҵ�������ݣ��а�ͷ�Ͱ���
			buf += uPackLength;
			pIO->m_uDataLength -= uPackLength;
		}
	}
}
