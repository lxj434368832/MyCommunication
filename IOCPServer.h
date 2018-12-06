#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "stdafx.h"
#include "IOCPDef.h"
#include "ResourceManage.h"
#include "MLock.h"

class INetInterface;

// 1�����������͵����Ӽ�����һ���ط���
// 2�������ʼ���������͵����ӡ�
// 3��ͳһ����iocp��ͳһ����iocp�̡߳�

class IOCPServier
{
  public:
	  IOCPServier(INetInterface *pSrvMng = nullptr);
	  virtual ~IOCPServier();

    bool InitIOCP(unsigned uThreadCount);

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
    bool StartServerListen(u_short port, unsigned iMaxServerCount);

	/*************************************************************************
	* function�� ��������
	* param key: �û�id
	* param data:��Ҫ���͵�����
	* return:	 ��
	*************************************************************************/
	void Send(int key, std::string data);

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

    //����
	  void PostSend(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

	  void DoSend(int iResult, PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO, DWORD dwBytesTransfered);


	//������յ�������
	  void UnpackReceivedData(PER_SOCKET_CONTEXT *pSkContext, PER_IO_CONTEXT* pIO);

private:
	//�����߳�
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

protected:
	enum 
	{
		SOCKET_CONTEXT_LOCK_COUNT = 100
	};
	const unsigned					m_uSocketContextLockCount = 100;
	HANDLE 							m_hIOCompletionPort;		//��ɶ˿�
	unsigned						m_uThreadCount;				//�̸߳���
	HANDLE				 			*m_aThreadList;				//�̳߳��б�
	PER_SOCKET_CONTEXT				*m_pListenSocketContext;	//����socket������
	std::map<SOCKET, PER_SOCKET_CONTEXT*>	m_mapConnectList;	//�����б�
	MLock	m_aLckSocketContext[SOCKET_CONTEXT_LOCK_COUNT];	//socket��������
	MLock									m_lckConnectList;		//�����б���
	mqw::ResourceManage<PER_SOCKET_CONTEXT>	m_rscSocketContext;	//socket��Դ����
    mqw::ResourceManage<PER_IO_CONTEXT>		m_rscIoContext;		//IO��Դ����
	INetInterface *m_pNetInterface;								//���������
};
