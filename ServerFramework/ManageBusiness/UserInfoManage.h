#pragma once
/*************************************************************************
* function�����������ӹ���Ķ����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../../IOCPCommunication/INetInterface.h"
#include "../../Framework/include/ResourceManage.h"
#include "../TypeDefine.h"
#include "../EnumDefine.h"

class IManageBusiness;
class ProtobufMsgFactory;

class UserInfoManage : public INetInterface
{
public:
	UserInfoManage(IManageBusiness* pMgr);
	virtual ~UserInfoManage();

	bool Start();
	void Stop();

	//�·����û��ɹ�����֪ͨ
	void AddUser(UserKey uUserKey) override;
	//�����������
	void HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length) override;
	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	void DeleteUser(UserKey uUserKey) override;

	/*************************************************************************
	* function����ȡ�ͻ����û���Ϣͨ��UserKey
	* return:	�ɹ�����ClientUserInfo��ʧ�ܷ���nullptr
	*************************************************************************/
	ClientUserInfo* GetClientUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function����ȡ�ͻ����û���Ϣͨ��UserId
	* return:	�ɹ�����ClientUserInfo��ʧ�ܷ���nullptr
	*************************************************************************/
	ClientUserInfo* GetClientUserInfoById(UserId uUserId);

	/*************************************************************************
	* function�����UserId��UserKey��ӳ���ϵ�������û���¼��ʱ�����
	* return:	��
	*************************************************************************/
	void AddUserIdUserKeyMap(UserId uUserId, UserKey uUserKey);

	/*************************************************************************
	* function������UserId�ͻ�ȡ��Ӧ��UserKey
	* return:	UserKey
	*************************************************************************/
	UserKey GetUserKeyById(UserId uUserId);

	/*************************************************************************
	* function������UserKey��ȡ��ǰ�û���Ӧ����
	* return:	UserKey
	*************************************************************************/
	MLock*	GetClientUserLock(UserKey uUserKey);

private:
	IManageBusiness*	m_pMgr;
	ProtobufMsgFactory* m_pMsgFctry;

	mqw::ResourceManage<ClientUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];
};
