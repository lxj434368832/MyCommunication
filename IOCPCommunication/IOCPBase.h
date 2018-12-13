#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/

#include "stdafx.h"
#include "IOCPDef.h"
#include "Framework/ResourceManage.h"
#include "Framework/MLock.h"

class INetInterface;

class IOCPBase
{
public:
	IOCPBase(INetInterface *pNet);
	virtual ~IOCPBase();

	bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	virtual bool StartServerListen(u_short port, unsigned iMaxConnectCount);

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	virtual bool StartConnect(unsigned uUserKey, std::string ip, u_short port, int iRecnnt = -1);

protected:
	//�������˲���
	void HandServerOperate(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	bool PostConnectEx(PER_SOCKET_CONTEXT *pSkContext);

	void DoConnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//Ͷ�ݽ���
	bool PostAcceptEx(SOCKET listenSocket);

	void DoAccept(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	//
	void PostReceive(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoReceive(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	//������յ�������
	void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(unsigned uUserKey, unsigned uMsgType, const char* data, unsigned uLength);

	//�������
	void PackSendData(PER_SOCKET_CONTEXT * pSkContext, unsigned uMsgType, const char* data, unsigned length);
	//����
	bool PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);

	void Disconnect(unsigned uUserKey);
	//
	void PostDisconnectEx(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	void DoDisconnect(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

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
	INetInterface *m_pNetInterface;								//����ӿ�
};

