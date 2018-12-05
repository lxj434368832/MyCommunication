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

class ConnectManage;
class ServerManage;
class ClientManage;

// 1�����������͵����Ӽ�����һ���ط���
// 2�������ʼ���������͵����ӡ�
// 3��ͳһ����iocp��ͳһ����iocp�̡߳�

class IOCPClient
{
  public:
	  IOCPClient(ConnectManage *pCnntMng = NULL, 
		  ServerManage *pSrvMng = NULL, ClientManage *pClientMng = NULL);
	  virtual ~IOCPClient();

    bool InitIOCP(unsigned uThreadCount);

    //�����߳�
	static DWORD WINAPI WorkerThread(LPVOID lpParameter);

	/*************************************************************************
	* function������һ��������Ҫ�����Ƿ���Ҫ����
	* param ip port: ��Ҫ���ӵ�ip�Ͷ˿ں�
	* param pIO:	 �������ʱҪ�������ݣ���Ҫ����pIO
	* param iRecnnt: �Ƿ�������ʶ,С��0������Ҫ����
	* return:		 ���ش����Ӷ�Ӧ��id,�����������ӳɹ���Ϊ0�������ӳ����˴���
	*************************************************************************/
	unsigned StartConnect(std::string ip, u_short port, int iRecnnt = -1, const char* data = nullptr, int len = 0);

    //�������Ӳ���
    void HandConnectOperate(int iResult, PER_IO_CONTEXT* pIO);

    //����ͻ��˲���
    void HandClientOperate(int iResult, PER_IO_CONTEXT* pIO);

    //�������˲���
    void HandServerOperate(int iResult, PER_IO_CONTEXT* pIO);

	/*************************************************************************
	* function����ȡһ��IOContext�Ľṹ
	* return:	IOContext��ָ��
	*************************************************************************/
    PER_IO_CONTEXT* GetIOContext();

	//��������IOContext
	void ReleaseIOContext(PER_IO_CONTEXT *pIO);

  private:
	  bool PostConnectEx(PER_IO_CONTEXT* pIO, SOCKADDR* serverAddr);

    //Ͷ�ݽ���
	  bool PostAcceptEx(SOCKET listenSocket, EOperateType op);

    //
	void PostDisconnectEx(PER_IO_CONTEXT* pIO, EOperateType op);

    //
    void PostReceive(PER_IO_CONTEXT* pIO, EOperateType op);

    //����
	void PostSend(PER_IO_CONTEXT* pIO, EOperateType op);

	//������յ�������
	void UnpackReceivedData(PER_IO_CONTEXT* pIO, std::function<void(unsigned, const char*, unsigned)> HandData);

	void DoReceive(PER_IO_CONTEXT* pIO, int iResult, std::function<void(unsigned, const char*, unsigned)> HandData);

private:
	HANDLE 							m_hIOCP;		//��ɶ˿�
	unsigned						m_uThreadCount;	//�̸߳���
	HANDLE				 			*m_aThreadList;	//�̳߳��б�
    mqw::ResourceManage<PER_IO_CONTEXT>	m_rscIO;		//IO��Դ����

	ConnectManage					*m_pCnntMng;	//���ӹ���
	ServerManage					*m_pSrvMng;		//����������
	ClientManage					*m_pClientMng;	//�ͻ��˹���
};

