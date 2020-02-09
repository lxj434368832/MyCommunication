#pragma once
/*************************************************************************
* function��iocp���������ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/

#include <WinSock2.h>
#include <MSWSock.h>
#include <queue>

#define MAX_BUF_LEN 512			//���ͻ�������С512B
#define MAX_RCV_BUF_LEN 5120	//���ջ�������С5K
#define CHECK_VALUE 0xfafbfcfd	//��������ֵ

// ����ɶ˿���Ͷ�ݵ�I/O����������
enum EOperateType
{
	EOP_UNKNOWN,
	EOP_CONNECT,
	EOP_ACCEPT,
	EOP_RECEIVE,
	EOP_SEND,
	EOP_DISCONNECT, 
	EOP_KEEP_LIVE
};

#pragma pack(push,1)
struct PackHeader
{
	unsigned		uCheck = CHECK_VALUE;	//��Ҫ���ڴ�����
	unsigned		uMsgType;
	unsigned long	ulBodyLength;
};
#pragma pack(pop)

//��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
struct PER_IO_CONTEXT
{
	OVERLAPPED	m_overlapped;
	SOCKET		m_socket;
	EOperateType m_oprateType;
	WSABUF		m_wsaBuf;
	char		*m_szBuffer;
	unsigned	m_uBufLength;
	unsigned	m_uDataLength;
	WPARAM		m_wParam;			// ��չ����
	LPARAM		m_lParam;			// ��չ����2

	PER_IO_CONTEXT(unsigned BufLen = MAX_BUF_LEN)
	{
		ZeroMemory(&m_overlapped, sizeof(m_overlapped));
		m_socket = INVALID_SOCKET;
		m_uBufLength = BufLen;
		m_szBuffer = new char[BufLen];

		Reset();
	}

	virtual ~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		m_szBuffer = nullptr;
		if (INVALID_SOCKET != m_socket)	//�ͷ�socket��Դ�����ܲ���Ҫ
		{ 
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	void Reset()
	{
		m_socket = INVALID_SOCKET;
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = 0;
		ZeroMemory(m_szBuffer, m_uBufLength);
		m_uDataLength = 0;	
		m_wParam = 0;
		m_lParam = 0;
	}
};

struct PER_RECEIVE_IO_CONTEXT :public PER_IO_CONTEXT
{
	PER_RECEIVE_IO_CONTEXT()
		:PER_IO_CONTEXT(MAX_RCV_BUF_LEN)
	{
	}
};

//====================================================================================
//
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================

struct PER_SOCKET_CONTEXT
{
	unsigned				m_uUserKey;					//ÿ��socket��Ӧ��UserKey
	SOCKET					m_socket;                   // ÿһ���ͻ������ӵ�Socket
	SOCKADDR_IN				m_clientAddr;               // �ͻ��˵ĵ�ַ
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//����������
	int						m_iDisconnectFlag;			//�Ͽ����ӱ�ʶ
	int						m_iSendPendingFlag;			//���������ʶ
	std::queue<PER_IO_CONTEXT*> m_queueIoContext;       //�ͻ��˷���������������¶���

	// ��ʼ��
	PER_SOCKET_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
	}

	// �ͷ���Դ
	~PER_SOCKET_CONTEXT()
	{
		if (m_socket != INVALID_SOCKET)
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

	void Reset()
	{
		//socket ���ⲿ�����˴�������
		m_uUserKey = 0;
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
		m_ReceiveContext.Reset();
		m_iDisconnectFlag = 0;
		m_iSendPendingFlag = 0;
		
		if(false == m_queueIoContext.empty())
		{
			printf("socket:%d�Ͽ��ܴ���%d��IO��Դй¶������ǰ�ͷţ�", m_socket, m_queueIoContext.size());
		}
	}
};



#include "..\Framework\include\ResourceManage.h"
#include "..\Framework\include\MLock.h"
#include "INetInterface.h"
#include <thread>

struct IOCPBaseData
{
	enum
	{
		SOCKET_RESOURCE_COUNT = 5,
		IO_RESOURCE_COUNT = 5,
		SOCKET_CONTEXT_LOCK_COUNT = 10
	};

	HANDLE 							hIOCompletionPort;		//��ɶ˿�
	unsigned						uThreadCount;			//�����̸߳���
	std::thread				 		*aThreadList;			//�����̳߳��б�

	std::map<unsigned, PER_SOCKET_CONTEXT*>	mapStayConnect; //�����Ӷ���
	MLock									lckStayConnect;	//�����Ӷ�����
	std::map<unsigned, PER_SOCKET_CONTEXT*>	mapConnectList;	//�����б�
	MLock									lckConnectList;	//�����б���

	MLock	aLckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];		//socket��������
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	rscSocketContext;	//socket��Դ����
	mqw::ResourceManage<PER_IO_CONTEXT>		rscIoContext;		//IO��Դ����
	INetInterface							*pNetInterface;		//����ӿ�
	std::thread								*pHeartbeatThread;	//�����߳�
	HANDLE 									hHeartbeatEvent;	//�����¼�

	IOCPBaseData(INetInterface *pNet) :
		pNetInterface(pNet),
		hIOCompletionPort(INVALID_HANDLE_VALUE),
		rscSocketContext(SOCKET_RESOURCE_COUNT),
		rscIoContext(IO_RESOURCE_COUNT)
	{
		uThreadCount = 0;
		aThreadList = NULL;
		pHeartbeatThread = NULL;
		hHeartbeatEvent = NULL;
	}

	~IOCPBaseData()
	{
		pNetInterface = nullptr;
	}
};
