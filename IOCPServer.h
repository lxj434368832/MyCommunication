#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company: 
*************************************************************************/

#include "stdafx.h"
#include "IOCPDef.h"
#include "ResourceManage.h"
#include "MLock.h"

class INetInterface;

class IOCPServier
{
public:
	IOCPServier(INetInterface *pSrvMng = nullptr);
	virtual ~IOCPServier();

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	bool StopServer();

	//������⺯���������̶߳�ʱ���á�
	void HeartbeatCheck();

	bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount);

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);
	
	void Disconnect(unsigned uUserKey);

protected:
	//�������˲���
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//Ͷ�ݽ���
	bool PostAcceptEx(SOCKET listenSocket);

	void DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//������յ�������
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);
	//�������
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//����
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);


private:
	//�����߳�
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

protected:
	enum 
	{
		SOCKET_CONTEXT_LOCK_COUNT = 100
	};

	HANDLE 							m_hIOCompletionPort;		//��ɶ˿�
	unsigned						m_uThreadCount;				//�̸߳���
	HANDLE				 			*m_aThreadList;				//�̳߳��б�
	PER_SOCKET_CONTEXT				*m_pListenSocketContext;	//����socket������
	std::map<SOCKET, PER_SOCKET_CONTEXT*>	m_mapConnectList;	//�����б�
	MLock									m_lckConnectList;	//�����б���

	MLock	m_aLckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];		//socket��������
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	m_rscSocketContext;	//socket��Դ����
    mqw::ResourceManage<PER_IO_CONTEXT>		m_rscIoContext;		//IO��Դ����
	INetInterface *m_pNetInterface;								//���������
};
