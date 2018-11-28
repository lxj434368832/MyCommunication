#include "stdafx.h"
#include "IOCPCommunication.h"
#include "IOCPModule.h"
#include "ConnectManage.h"
#include "ServerManage.h"
#include "ClientManage.h"
#include <WS2tcpip.h>

IOCPCommunication::IOCPCommunication(ConnectManage *pCnntMng,
	ServerManage *pSrvMng, ClientManage *pClientMng):
	 m_hIOCP(INVALID_HANDLE_VALUE), m_rscIO(1000)
{
	m_uThreadCount = 0;
	m_aThreadList = NULL;

	m_pCnntMng = pCnntMng;
	m_pSrvMng = pSrvMng;
	m_pClientMng = pClientMng;
}

IOCPCommunication::~IOCPCommunication()
{
	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		IOCPModule::Instance()->PostQueuedCompletionStatus(m_hIOCP, 0, NULL, NULL);
	}

	for (unsigned i = 0; i < m_uThreadCount; i++)
	{
		::WaitForSingleObject(m_aThreadList[i], INFINITE);
		::CloseHandle(m_aThreadList[i]);
	}

	RELEASE_HANDLE(m_hIOCP);
	delete[] m_aThreadList;
	m_pCnntMng = NULL;
	m_pSrvMng = NULL;
	m_pClientMng = NULL;
}

bool IOCPCommunication::InitIOCP(unsigned uThreadCount)
{
	assert(uThreadCount);

	m_hIOCP = IOCPModule::Instance()->CreateIoCompletionPort();
	if (NULL == m_hIOCP)
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

DWORD WINAPI  IOCPCommunication::WorkerThread(LPVOID lpParameter)
{
	IOContext *pIO = NULL;
	int iResult = 0;
	IOCPCommunication *pThis = (IOCPCommunication*)lpParameter;
	while (true)
	{
		iResult = IOCPModule::Instance()->GetQueuedCompletionStatus(pThis->m_hIOCP, &pIO);
		if (pIO)
		{
			switch (pIO->oprateType)
			{
			case EOP_CNNT_CONNECT:
			case EOP_CNNT_DISCONNECT:
			case EOP_CNNT_RECEIVE:
			case EOP_CNNT_SEND:
				pThis->HandConnectOperate(iResult, pIO);
				break;

			case EOP_CLIENT_ACCEPT:
			case EOP_CLIENT_DISCONNECT:
			case EOP_CLIENT_RECEIVE:
			case EOP_CLIENT_SEND:
				pThis->HandClientOperate(iResult, pIO);
				break;

			case EOP_SRV_ACCEPT:
			case EOP_SRV_DISCONNECT:
			case EOP_SRV_RECEIVE:
			case EOP_SRV_SEND:
				pThis->HandServerOperate(iResult, pIO);
				break;

			default:
				MLOG("δ֪�Ĳ������ͣ�%d", pIO->oprateType);
				MAssert(false);
				break;
			}
		}
		else
		{
			MLOG("�߳�:%d������", ::GetCurrentThreadId());
			break;
		}
	}

	return 0;
}

unsigned IOCPCommunication::StartConnect(std::string ip, u_short port, IOContext* pIO, int iRecnnt)
{
	unsigned uUserId = 0;
	do 
	{
		if (NULL == pIO)
			pIO = m_rscIO.get();

		pIO->wParam = iRecnnt;

		if (INVALID_SOCKET == pIO->socket)
		{
			if (INVALID_SOCKET == (pIO->socket = IOCPModule::Instance()->Socket()))
			{
				MLOG("����socketʧ�ܣ������룺%d", WSAGetLastError());
				break;
			}
			MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pIO->socket, m_hIOCP));
		}

		SOCKADDR_IN addrLocal;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_port = 0;
		MAssert(0 == IOCPModule::Instance()->Bind(pIO->socket, (LPSOCKADDR)&addrLocal));

		if (NULL == m_pCnntMng)
		{
			MLOG("���ʼ��ConnectManage!");
			break;
		}

		SOCKADDR_IN *pAddrSrv = m_pCnntMng->GetSOCKADDR();
		MAssert(pAddrSrv);
		pAddrSrv->sin_family = AF_INET;
		inet_pton(AF_INET, ip.c_str(), &pAddrSrv->sin_addr);
		pAddrSrv->sin_port = htons(port);
		pIO->lParam = (LPARAM)pAddrSrv;

		pIO->uUserId = m_pCnntMng->GetCnntUserId();
		PostConnectEx(pIO, (LPSOCKADDR)pAddrSrv);
		uUserId = pIO->uUserId;		//��Ϊ����ʧ��ʱpIO->uUserId=0

	} while (0);
	return uUserId;
}

bool IOCPCommunication::StartServerListen(u_short port, unsigned iMaxServerCount)
{
	bool bRet = false;
	SOCKET lstnSocket = INVALID_SOCKET;
	do 
	{
		lstnSocket = IOCPModule::Instance()->Socket();
		if (INVALID_SOCKET == lstnSocket)
		{
			MLOG("����Server����socketʧ�ܣ������룺%d", WSAGetLastError());
			break;
		}
		int nOpt = 1;
		setsockopt(lstnSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nOpt, sizeof(nOpt));

		SOCKADDR_IN addrLocal;
		addrLocal.sin_family = AF_INET;
		addrLocal.sin_addr.s_addr = ADDR_ANY;
		addrLocal.sin_port = htons(port);
		if(IOCPModule::Instance()->Bind(lstnSocket, (LPSOCKADDR)&addrLocal)) break;

		if (IOCPModule::Instance()->Listen(lstnSocket, SOMAXCONN)) break;

		if (IOCPModule::Instance()->BindIoCompletionPort(lstnSocket, m_hIOCP)) break;

		//Ͷ�ݽ��ܲ���
		for (unsigned i = 0; i < iMaxServerCount; i++)
		{
			if (false == (bRet = PostAcceptEx(lstnSocket, EOP_SRV_ACCEPT)))
			{
				break;
			}
		}

	} while (0);

	if (bRet)
	{
		m_pSrvMng->SetSrvListenSocket(lstnSocket);
	}
	else
	{
		closesocket(lstnSocket);
	}
	
	return bRet;
}

bool IOCPCommunication::StartClientListen(u_short port, unsigned iMaxUserCount)
{
	return true;
}

void IOCPCommunication::HandConnectOperate(int iResult, IOContext* pIO)
{
	switch (pIO->oprateType)
	{
	case EOP_CNNT_CONNECT:
		m_pCnntMng->ConnectNotify(pIO->uUserId, pIO->socket, 0 == iResult);

		if (0 == iResult)
		{
			PostReceive(pIO, EOP_CNNT_RECEIVE);
		}
		else
		{
			if (ERROR_CONNECTION_REFUSED == iResult)	//��������û������1225L
				MLOG("����ʧ�ܣ������û������!");
			else
				MLOG("����ʧ�ܣ������룺%d", iResult);
		
			if ((int)pIO->wParam > 0)
			{
				MLOG("socket:%d��������!", pIO->socket);
				Sleep(1000);
				PostConnectEx(pIO, (LPSOCKADDR)pIO->lParam);
			}
			else
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
				ReleaseIOContext(pIO);
			}
		}

		break;
	case EOP_CNNT_DISCONNECT:
		if (0 == iResult)
		{
			if ((int)pIO->wParam > 0)	//��Ҫ����
			{
				MLOG("socket:%d��������!", pIO->socket);
				PostConnectEx(pIO, (LPSOCKADDR)pIO->lParam);
			}
			else
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
				ReleaseIOContext(pIO);
			}
		}
		else	//�˴���Ӧ�ý���
		{
			MLOG("�Ͽ�����ʧ�ܣ������룺%d", iResult);
			MAssert(false);
		}

		break;
	case EOP_CNNT_RECEIVE:
		if (0 == pIO->overlapped.InternalHigh)	//����������ѶϿ���socket����
		{
			MLOG("�������һ���ѶϿ�socket��%d �����ӣ�",pIO->socket, iResult);
			PostDisconnectEx(pIO, pIO->oprateType);
		}
		else if (0 == iResult)
		{
			UnpackReceivedData(pIO, std::bind(&ConnectManage::HandCnntData, m_pCnntMng,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}
		else
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("�������һ�������ߣ�");
			else
				MLOG("connect����ʧ�ܣ������룺%d", iResult);

			PostDisconnectEx(pIO, pIO->oprateType);
		}

		break;
	case EOP_CNNT_SEND:
		if (0 == pIO->overlapped.InternalHigh)
		{
			MLOG("�������һ���ѶϿ�socket��%d �����ӣ�");
			shutdown(pIO->socket, SD_BOTH);
		}
		else if (iResult)
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("�������һ�������ߣ�");
			else
				MLOG("connect����ʧ�ܣ������룺%d", iResult);

			shutdown(pIO->socket, SD_BOTH);
		}

		ReleaseIOContext(pIO);

		break;
	default:
		MLOG("δ֪�����Ӳ����룺%d", pIO->oprateType);
		MAssert(false);
		break;
	}
}

void IOCPCommunication::HandClientOperate(int iResult, IOContext* pIO)
{

}

void IOCPCommunication::HandServerOperate(int iResult, IOContext* pIO)
{
	switch (pIO->oprateType)
	{
	case EOP_SRV_ACCEPT:
		if (0 == iResult)	//Ͷ��AcceptEx�ɹ�
		{
			pIO->uUserId = m_pSrvMng->GetSrvUserId();
			m_pSrvMng->SrvAcceptNotify(pIO->uUserId, pIO->socket);
			pIO->oprateType = EOP_SRV_RECEIVE;
			UnpackReceivedData(pIO, std::bind(&ServerManage::HandSrvData, m_pSrvMng,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

			SOCKADDR_IN *pUserAddr = NULL;
			IOCPModule::Instance()->GetAcceptExSockaddrs(pIO, (LPSOCKADDR*)&pUserAddr);
			char strIP[255];	inet_ntop(AF_INET, &pUserAddr->sin_addr, strIP, 255);
			MLOG("���û����롣�û�IP��%s���û��׽��֣�%d", strIP, pIO->socket);

			PostAcceptEx(m_pSrvMng->GetSrvListenSocket(), EOP_SRV_ACCEPT);	//
		}
		else
		{
			MLOG("Ͷ��AcceptExʧ�ܣ������룺%d", iResult);
			ReleaseIOContext(pIO);			//������д���֤
		}
		break;

	case EOP_SRV_DISCONNECT:
		if (iResult)
		{
			MLOG("Server�Ͽ�����ʧ�ܣ�������:%d", iResult);
		}
		ReleaseIOContext(pIO);
		break;

	case EOP_SRV_RECEIVE:
		DoReceive(pIO, iResult, std::bind(&ServerManage::HandSrvData, m_pSrvMng,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		break;

	case EOP_SRV_SEND:
		if (0 == pIO->overlapped.InternalHigh)
		{
			MLOG("�������һ���ѶϿ�socket��%d �����ӣ�");
			shutdown(pIO->socket, SD_BOTH);
		}
		else if (iResult)
		{
			if (ERROR_NETNAME_DELETED == iResult)
				MLOG("�������һ�������ߣ�");
			else
				MLOG("SRV����ʧ�ܣ������룺%d", iResult);

			shutdown(pIO->socket, SD_BOTH);
		}

		ReleaseIOContext(pIO);

		break;
	default:
		MLOG("δ֪�ķ�������룺%d", pIO->oprateType);
		MAssert(false);
		break;
	}
}

void IOCPCommunication::DoReceive(IOContext* pIO, int iResult,std::function<void(unsigned, const char*, unsigned)> HandData)
{
	if (0 == pIO->overlapped.InternalHigh)	//����������ѶϿ���socket����
	{
		MLOG("�������һ���ѶϿ�socket��%d �����ӣ������룺%d", pIO->socket, iResult);
		PostDisconnectEx(pIO, pIO->oprateType);
	}
	else if (0 == iResult)
	{
		UnpackReceivedData(pIO, HandData);
	}
	else
	{
		if (ERROR_NETNAME_DELETED == iResult)
			MLOG("�������һ�������ߣ�");
		else
			MLOG("SRV����ʧ�ܣ������룺%d", iResult);

		PostDisconnectEx(pIO, pIO->oprateType);
	}
}

IOContext* IOCPCommunication::GetIOContext()
{
	return m_rscIO.get();
}

void IOCPCommunication::ReleaseIOContext(IOContext *pIO)
{
	pIO->reset();
	m_rscIO.put(pIO);
}

//
void IOCPCommunication::UnpackReceivedData(IOContext* pIO, std::function<void(unsigned, const char*, unsigned)> HandData)
{
	pIO->dataLength += pIO->overlapped.InternalHigh;
	char *buf = pIO->buffer;

	PackHeader *head = NULL;
	unsigned uDataLength = 0;
	unsigned uPackLength = 0;

	while (true)
	{
		head = (PackHeader*)buf;
		uPackLength = sizeof(PackHeader) + head->ulBodyLength;
		uDataLength = pIO->dataLength;

		if (uDataLength >= sizeof(PackHeader) && uPackLength > MAX_BUF_LEN)	//���յ������ݲ���ȷ
		{
			MLOG("���յ������ݳ���̫�󣬶Ͽ����ӣ�");
			PostDisconnectEx(pIO, pIO->oprateType);
			break;
		}
		else if (uDataLength < sizeof(PackHeader) || uDataLength < uPackLength)	//���� һ����ͷ����һ������
		{
			if (buf != pIO->buffer)
			{
				memmove(pIO->buffer, buf, uDataLength);
			}
			pIO->wsabuf.buf = pIO->buffer + uDataLength;
			pIO->wsabuf.len = pIO->bufLength - uDataLength;
			PostReceive(pIO, pIO->oprateType);
			break;
		}
		else //������һ�����������������ʼ���
		{
			HandData(pIO->uUserId, buf, uPackLength);	//���þ����ҵ�������ݣ��а�ͷ�Ͱ���
			buf += uPackLength;
			pIO->dataLength -= uPackLength;
		}
	}
}

bool IOCPCommunication::PostConnectEx(IOContext* pIO, SOCKADDR* pSrvAddr)
{
	MAssert(pIO && pSrvAddr);
	pIO->oprateType = EOP_CNNT_CONNECT;
	if (IOCPModule::Instance()->ConnectEx(pIO, pSrvAddr))
	{
		MAssert(false);
		m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
		ReleaseIOContext(pIO);

		return false;
	}
	return true;
}

bool IOCPCommunication::PostAcceptEx(SOCKET listenSocket, EOperateType op)
{
	if (INVALID_SOCKET == listenSocket)
	{
		MLOG("listenSocket��Ч��");
		return false;
	}
	else if (op != EOP_CLIENT_ACCEPT || op != EOP_SRV_ACCEPT)
	{
		MLOG("Ͷ�ݵĲ������ʹ���");
		return false;
	}

	IOContext *pIO = GetIOContext();
	if (INVALID_SOCKET == pIO->socket)
	{
		if (INVALID_SOCKET == (pIO->socket = IOCPModule::Instance()->Socket()))
		{
			MLOG("����socketʧ�ܣ������룺%d", WSAGetLastError());
			return false;
		}
		MAssert(0 == IOCPModule::Instance()->BindIoCompletionPort(pIO->socket, m_hIOCP));
	}

	pIO->oprateType = op;
	if (IOCPModule::Instance()->AcceptEx(listenSocket, pIO))	//ʧ��
	{
		ReleaseIOContext(pIO);
		return false;
	}
	else
		return true;
}

void IOCPCommunication::PostDisconnectEx(IOContext* pIO, EOperateType op)
{
	UserInfo *pInfo = NULL;

	if (op < EOP_CNNT_OPERATE)
	{
		pInfo = m_pCnntMng->DeleteCnntUser(pIO->uUserId);
		op = EOP_CNNT_DISCONNECT;
	}
	else if (op < EOP_CLIENT_OPRATE)
	{
		pInfo = m_pClientMng->DeleteClientUser(pIO->uUserId);
		op = EOP_CLIENT_DISCONNECT;
	}
	else if (op < EOP_SRV_OPERATE)
	{
		pInfo = m_pSrvMng->DeleteSrvUser(pIO->uUserId);
		op = EOP_SRV_DISCONNECT;
	}
	else
	{
		MLOG("δ֪�Ĳ�������,ֵΪ��%d", op);
	}

	if (pInfo)
	{
		pIO->oprateType = op;
		if (IOCPModule::Instance()->DisconnectEx(pIO))
		{
			if (op < EOP_CNNT_OPERATE && pIO->lParam)
			{
				m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
			}

			ReleaseIOContext(pIO);
		}
	}
	else
	{
		MLOG("�û������ڣ���Ӧ��ִ�е�������ע�⣡");

		if (op < EOP_CNNT_OPERATE && pIO->lParam)
		{
			m_pCnntMng->RecycleSOCKADDR((LPSOCKADDR_IN)pIO->lParam);
		}

		ReleaseIOContext(pIO);
	}

}

void IOCPCommunication::PostReceive(IOContext* pIO, EOperateType op)
{
	pIO->oprateType = op;
	if (IOCPModule::Instance()->Receive(pIO))	//����ʧ�ܵĴ���
	{
		MAssert(false);
		shutdown(pIO->socket, SD_BOTH);
		PostDisconnectEx(pIO, op);
	}
}

void IOCPCommunication::PostSend(IOContext* pIO, EOperateType op)
{
	pIO->oprateType = op;
	if (IOCPModule::Instance()->Send(pIO))	//����ʧ�ܵĴ���
	{
		MAssert(false);
		shutdown(pIO->socket, SD_BOTH);	//�����Ǳ߻��յ�����
		ReleaseIOContext(pIO);
	}

}

