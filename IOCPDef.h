#pragma once
/*************************************************************************
* function��iocp���������ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "stdafx.h"
#include <afxtempl.h>

#define MAX_BUF_LEN 512 //512B
#define RECEIVE_BUF_COUNT 10


// ����ɶ˿���Ͷ�ݵ�I/O����������
enum EOperateType
{
	EOP_UNKNOWN,
	EOP_CNNT_CONNECT,
	EOP_CNNT_DISCONNECT,
	EOP_CNNT_RECEIVE,
	EOP_CNNT_SEND,
	EOP_CNNT_OPERATE,
	EOP_CLIENT_ACCEPT,
	EOP_CLIENT_DISCONNECT,
	EOP_CLIENT_RECEIVE,
	EOP_CLIENT_SEND,
	EOP_CLIENT_OPRATE,
	EOP_SRV_ACCEPT,
	EOP_SRV_DISCONNECT,
	EOP_SRV_RECEIVE,
	EOP_SRV_SEND,
	EOP_SRV_OPERATE
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

		reset();
	}

	~PER_IO_CONTEXT()
	{
		delete[] m_szBuffer;
		RELEASE_SOCKET(m_socket);		//�ͷ�socket��Դ�����ܲ���Ҫ
	}
	void reset()
	{
		ZeroMemory(&m_overlapped, sizeof(m_overlapped));
		ZeroMemory(m_szBuffer, MAX_BUF_LEN);
		m_oprateType = EOP_UNKNOWN;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = m_uBufLength;
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
		m_uBufLength = MAX_BUF_LEN * RECEIVE_BUF_COUNT;
		m_szBuffer = new char[m_uBufLength];

		reset();
	}
};

//====================================================================================
//
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================

typedef struct _PER_SOCKET_CONTEXT
{
	SOCKET					m_Socket;                   // ÿһ���ͻ������ӵ�Socket
	SOCKADDR_IN				m_ClientAddr;               // �ͻ��˵ĵ�ַ
	PER_RECEIVE_IO_CONTEXT	m_ReceiveContext;			//����������

	CArray<PER_IO_CONTEXT*> m_arrayIoContext;             // �ͻ���������������������ݣ�
														   // Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����

	// ��ʼ��
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
	}

	// �ͷ���Դ
	~_PER_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		// �ͷŵ����е�IO����������
		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			delete m_arrayIoContext.GetAt(i);
		}
		m_arrayIoContext.RemoveAll();
	}

	// ��ȡһ���µ�IoContext
	PER_IO_CONTEXT* GetNewIoContext()
	{
		PER_IO_CONTEXT* p = new PER_IO_CONTEXT;

		m_arrayIoContext.Add(p);

		return p;
	}

	// ���������Ƴ�һ��ָ����IoContext
	void RemoveContext(PER_IO_CONTEXT* pContext)
	{
		ASSERT(pContext != NULL);

		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			if (pContext == m_arrayIoContext.GetAt(i))
			{
				delete pContext;
				pContext = NULL;
				m_arrayIoContext.RemoveAt(i);
				break;
			}
		}
	}

}PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;
