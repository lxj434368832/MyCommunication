#pragma once
/*************************************************************************
* function��iocpͨѶ�����ļ�
* author :	������
* datetime��2017-12-14
* company: 
*************************************************************************/

#include "IOCPBase.h"

class INetInterface;

class IOCPServier : public IOCPBase
{
public:
	IOCPServier(INetInterface *pSrvMng = nullptr);
	virtual ~IOCPServier();

	bool StartServer(USHORT nPort, unsigned dwMaxConnection = 10, unsigned uThreadCount = 0);
	bool StopServer();

	//������⺯���������̶߳�ʱ���á�
	void HeartbeatCheck();

	/*************************************************************************
	* function��  ������Է���˵ļ���
	* param port: ���ؼ����Ķ˿ں�
	* param iMaxServerCount:�������Ӹ���
	* return:	  �ɹ�����true,ʧ�ܷ���false.
	*************************************************************************/
	bool StartServerListen(u_short port, unsigned iMaxConnectCount);

};
