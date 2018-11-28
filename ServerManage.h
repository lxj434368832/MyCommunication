#ifndef SERVERMANAGE_H_HEADER_INCLUDED_A5CF6A84
#define SERVERMANAGE_H_HEADER_INCLUDED_A5CF6A84
/*************************************************************************
* function�����������ӹ���Ķ����ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "MLock.h"
#include <atomic>
#include <map>

struct UserInfo;

class ServerManage
{
public:
	virtual ~ServerManage();
	void SetSrvListenSocket(SOCKET listenSocket);

	SOCKET GetSrvListenSocket();

	unsigned GetSrvUserId();

	//�·����û��ɹ�����֪ͨ
	virtual void SrvAcceptNotify(unsigned uUserId, SOCKET s);

	//�����������
	virtual void HandSrvData(unsigned uUserId, const char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	UserInfo* DeleteSrvUser(unsigned userId);

protected:
	virtual UserInfo* CreateSrvUserInfo() = 0;

	virtual void ReleaseSrvUserInfo(UserInfo* pSrvUserInfo) = 0;

protected:
	SOCKET m_sSrvLstnSocket;

	std::map<unsigned, UserInfo*> m_mapSrvUserList;	//�����û��б�
	MLock						 m_lckSrvUserList;	//�����û��б���

private:
	std::atomic<unsigned> m_uSrvUidGenerator = 1;
};



#endif /* SERVERMANAGE_H_HEADER_INCLUDED_A5CF6A84 */
