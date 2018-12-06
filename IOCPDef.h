#pragma once
/*************************************************************************
* function��iocp���������ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "stdafx.h"
#include <queue>

#define MAX_BUF_LEN 512			//���ͻ�������С512B
#define MAX_RCV_BUF_LEN 5120	//���ջ�������С5K

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
	unsigned long ulBodyLength;
	int			  msgType;
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

	PER_IO_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		m_uBufLength = MAX_BUF_LEN;
		m_szBuffer = new char[MAX_BUF_LEN];

		Reset();
	}

	virtual ~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		RELEASE_SOCKET(m_socket);		//�ͷ�socket��Դ�����ܲ���Ҫ
	}
	void Reset()
	{
		m_socket = INVALID_SOCKET;
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = m_uBufLength;
		ZeroMemory(m_szBuffer, MAX_BUF_LEN);
		m_uDataLength = 0;
		m_wParam = 0;
		m_lParam = 0;
	}
};

struct PER_RECEIVE_IO_CONTEXT :public PER_IO_CONTEXT
{
	PER_RECEIVE_IO_CONTEXT()
	{
		m_socket = INVALID_SOCKET;
		m_uBufLength = MAX_RCV_BUF_LEN;
		m_szBuffer = new char[MAX_RCV_BUF_LEN];

		Reset();
	}
};

//====================================================================================
//
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================

struct PER_SOCKET_CONTEXT
{
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
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
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
		memset(&m_clientAddr, 0, sizeof(m_clientAddr));
		m_ReceiveContext.Reset();
		m_iDisconnectFlag = 0;
		m_iSendPendingFlag = 0;
		
		if(false == m_queueIoContext.empty())
		{
			MLOG("socket:%d�Ͽ��ܴ���%d��IO��Դй¶������ǰ�ͷţ�", m_socket, m_queueIoContext.size());
		}
	}
};
