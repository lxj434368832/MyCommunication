#ifndef IOCPCOMMUNICATION_H_HEADER_INCLUDED_A5CF6C7D
#define IOCPCOMMUNICATION_H_HEADER_INCLUDED_A5CF6C7D
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

class IOCPCommunication
{
  public:
	  IOCPCommunication(ConnectManage *pCnntMng = NULL, 
		  ServerManage *pSrvMng = NULL, ClientManage *pClientMng = NULL);
	  virtual ~IOCPCommunication();

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
	unsigned StartConnect(std::string ip, u_short port, IOContext* pIO = NULL, int iRecnnt = -1);

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
    bool StartServerListen(u_short port, unsigned iMaxServerCount);

	/*************************************************************************
	* function��  ������Կͻ��˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
    bool StartClientListen(u_short port, unsigned iMaxUserCount);

    //�������Ӳ���
    void HandConnectOperate(int iResult, IOContext* pIO);

    //����ͻ��˲���
    void HandClientOperate(int iResult, IOContext* pIO);

    //�������˲���
    void HandServerOperate(int iResult, IOContext* pIO);

	/*************************************************************************
	* function����ȡһ��IOContext�Ľṹ
	* return:	IOContext��ָ��
	*************************************************************************/
    IOContext* GetIOContext();

	//��������IOContext
	void ReleaseIOContext(IOContext *pIO);

  private:
	  bool PostConnectEx(IOContext* pIO, SOCKADDR* serverAddr);

    //Ͷ�ݽ���
	  bool PostAcceptEx(SOCKET listenSocket, EOperateType op);

    //
	void PostDisconnectEx(IOContext* pIO, EOperateType op);

    //
    void PostReceive(IOContext* pIO, EOperateType op);

    //����
	void PostSend(IOContext* pIO, EOperateType op);

	//������յ�������
	void UnpackReceivedData(IOContext* pIO, std::function<void(unsigned, const char*, unsigned)> HandData);

	void DoReceive(IOContext* pIO, int iResult, std::function<void(unsigned, const char*, unsigned)> HandData);

private:
	HANDLE 							m_hIOCP;		//��ɶ˿�
	unsigned						m_uThreadCount;	//�̸߳���
	HANDLE				 			*m_aThreadList;	//�̳߳��б�
    mqw::ResourceManage<IOContext>	m_rscIO;		//IO��Դ����

	ConnectManage					*m_pCnntMng;	//���ӹ���
	ServerManage					*m_pSrvMng;		//����������
	ClientManage					*m_pClientMng;	//�ͻ��˹���
};



#endif /* IOCPCOMMUNICATION_H_HEADER_INCLUDED_A5CF6C7D */
