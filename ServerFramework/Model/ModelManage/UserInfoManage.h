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
#include <vector>

class IModelManage;

class UserInfoManage
{
public:
	UserInfoManage(IModelManage* pMgr);
	virtual ~UserInfoManage();

	bool Initialize();
	void Uninitialize();

	//�·����û��ɹ�����֪ͨ
	void AddUser(UserKey uUserKey);

	//ͨѶ��ص�ɾ���û�
	void DeleteUser(UserKey uUserKey);

	/*************************************************************************
	* function����ȡ�ͻ����û���Ϣͨ��UserKey
	* return:	�ɹ�����ClientUserInfo��ʧ�ܷ���nullptr
	*************************************************************************/
	SUserInfo* GetClientUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function����ȡ�ͻ����û���Ϣͨ��UserId
	* return:	�ɹ�����ClientUserInfo��ʧ�ܷ���nullptr
	*************************************************************************/
	SUserInfo* GetClientUserInfoById(UserId uUserId);

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
	* function����סUserKey��Ӧ����Ϣ
	*************************************************************************/
	void LockUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function������UserKey��Ӧ����Ϣ
	*************************************************************************/
	void UnlockUserInfo(UserKey uUserKey);

	std::vector<unsigned> GetOfflineUserList();

private:
	IModelManage*	m_pMgr;

	mqw::ResourceManage<SUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserId, UserKey>		m_mapIdKey;			//�û�id��key��ӳ���ϵ
	MLock							m_lckIdKey;			//�û�id��key��ӳ���ϵ��
	std::map<UserKey, SUserInfo*>	m_mapUserList;	//�û�key��UserInfo��ӳ��
	MLock							m_lckUserList;		//�û��б���

};
