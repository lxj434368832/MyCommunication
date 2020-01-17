#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company: 
*************************************************************************/

#include "IOCPBase.h"

class IOCPServer : public IOCPBase
{
public:
	IOCPServer(INetInterface *pNet = nullptr);
	virtual ~IOCPServer();

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	void StopServer();

	//������⺯���������̶߳�ʱ���á�
	void StartHeartbeatCheck();

private:
	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount);

private:
	bool						m_bStart;
	PER_SOCKET_CONTEXT			*m_pListenSocketContext;	//����socket������
};
