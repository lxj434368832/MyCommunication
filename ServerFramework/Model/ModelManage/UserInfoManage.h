#pragma once
/*************************************************************************
* function�����������ӹ���Ķ����ļ�
* author :	������
* datetime��2017-12-14
* company:  
*************************************************************************/
#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "../../3rdParty/Framework/include/MLock.h"
#include "../../CommonFile/TypeDefine.h"
#include "../../CommonFile/EnumDefine.h"
class IModelManage;

class UserInfoManage
{
public:
	UserInfoManage(IModelManage* pMgr);
	virtual ~UserInfoManage();

	bool Start();
	void Stop();

	//�·����û��ɹ�����֪ͨ
	void AddUser(UserKey uUserKey);

	//ͨѶ��ص�ɾ���û�
	void DeleteUser(UserKey uUserKey);

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

	void SetUserInfo(UserKey uUserKey, ClientUserInfo &info);

private:
	IModelManage*	m_pMgr;

	mqw::ResourceManage<ClientUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserId, UserKey>		m_mapIdKey;			//�û�id��key��ӳ���ϵ
	MLock							m_lckIdKey;			//�û�id��key��ӳ���ϵ��
	std::map<UserKey, ClientUserInfo*>	m_mapUserList;	//�û�key��UserInfo��ӳ��
	MLock							m_lckUserList;		//�û��б���

};
