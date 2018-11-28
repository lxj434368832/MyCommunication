#ifndef CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A
#define CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A
/*************************************************************************
* function�����ӹ������ļ�
* author :	������
* datetime��2017-12-14
* company:  ���̽ݿƼ��ɷ����޹�˾
*************************************************************************/

#include "ResourceManage.h"
#include <atomic>
#include <map>
#include "MLock.h"

struct UserInfo;

class ConnectManage
{
public:
	ConnectManage();
	virtual ~ConnectManage();

	unsigned GetCnntUserId();

	virtual void ConnectNotify(unsigned uUserId, SOCKET socket, bool bSuccess);

	// ����ɾ���û��ĵ�ַ������NULL�����û�������
	virtual UserInfo* DeleteCnntUser(unsigned UserId);

	// �˺���Ϊ�麯������Ҫ����ʵ�־����ҵ��
	virtual void HandCnntData(unsigned uUserId, const char* data, unsigned length);

	SOCKADDR_IN* GetSOCKADDR();

	void RecycleSOCKADDR(SOCKADDR_IN* addr);


protected:
	virtual UserInfo* CreateCnntUserInfo() = 0;

	virtual void ReleaseCnntUserInfo(UserInfo* pUserInfo) = 0;

protected:
	std::map<unsigned, UserInfo*> m_mapCnntUserList;		//�Ѿ����ӵ��û�����
	MLock						  m_lckUserList;			//�û��б���

private:
	std::atomic<unsigned> m_uCnntUserIdGenerator = 1;

	mqw::ResourceManage<SOCKADDR_IN> m_rscAddr;
};


#endif /* CONNECTMANAGE_H_HEADER_INCLUDED_A5CF3E5A */
