#include "UserInfoManage.h"
#include "IModelManage.h"
#include "..\..\CommonFile\CommonDefine.h"

UserInfoManage::UserInfoManage(IModelManage* pMgr)
	:m_rscUser(USER_RESOURCE_COUNT)
{
	m_pMgr = pMgr;
}

UserInfoManage::~UserInfoManage()
{
	m_pMgr = nullptr;
}

bool UserInfoManage::Start()
{
	if (nullptr == m_pMgr)
	{
		loge() << "m_pMgr Ϊ��";
		return false;
	}

	return true;
}

void UserInfoManage::Stop()
{

	//m_mapIdKey.clear();				//�û�id��key��ӳ���ϵ
	//m_mapUserList.clear();			//�û�key��UserInfo��ӳ��
}

void UserInfoManage::AddUser(UserKey uUserKey)
{
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (iter != m_mapUserList.end() && iter->second)
	{
		m_lckUserList.unlock();
		loge() << "�Ѿ����ڵ�ǰ�û�key:" << uUserKey <<"id:"<< iter->second->m_uUserId << "�������ӣ�";
		return;
	}

	ClientUserInfo *pUser = m_rscUser.get();
	pUser->Reset();
	pUser->m_uUserKey = uUserKey;
	m_mapUserList[uUserKey] = pUser;
	m_lckUserList.unlock();

	logm() << "�����ӵ���userKey:" << uUserKey;
}

void UserInfoManage::DeleteUser(UserKey uUserKey)
{
	ClientUserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() != iter)
	{
		pUser = static_cast<ClientUserInfo*>(iter->second);
	}
	m_mapUserList.erase(uUserKey);
	m_lckUserList.unlock();

	if (pUser)
	{
		m_lckIdKey.lock();
		m_mapIdKey.erase(pUser->m_uUserId);
		m_lckIdKey.unlock();

		m_rscUser.put(pUser);
		logm() << "���ӶϿ�userKey:" << uUserKey;
	}
}

ClientUserInfo* UserInfoManage::GetClientUserInfo(UserKey uUserKey)
{
	ClientUserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() == iter)
	{
		pUser = static_cast<ClientUserInfo*>(iter->second);
	}
	m_lckUserList.unlock();

	return pUser;
}

ClientUserInfo* UserInfoManage::GetClientUserInfoById(UserId uUserId)
{
	ClientUserInfo* pUser = nullptr;
	UserKey uKey = 0;

	m_lckIdKey.lock();
	auto iter = m_mapIdKey.find(uUserId);
	if (m_mapIdKey.end() != iter)
	{
		uKey = iter->second;
	}
	m_lckIdKey.unlock();
	if (0 == uKey)	return pUser;

	m_lckUserList.lock();
	auto iter1 = m_mapUserList.find(uKey);
	if (m_mapUserList.end() != iter1)
	{
		pUser = static_cast<ClientUserInfo*>(iter1->second);
	}
	m_lckUserList.unlock();

	return pUser;
}

void UserInfoManage::AddUserIdUserKeyMap(UserId uUserId, UserKey uUserKey)
{
	m_lckIdKey.lock();
	auto iter = m_mapIdKey.find(uUserId);
	if (m_mapIdKey.end() != iter && iter->second)
	{
		m_lckIdKey.unlock();
		loge() << "����UserId:" << uUserId << "��Ӧ��UserKey:" << iter->second 
			<< "��UserKey:" << uUserKey << "����ʧ�ܣ�";
		return;
	}
	m_mapIdKey[uUserId] = uUserKey;
	m_lckIdKey.unlock();
}

UserKey UserInfoManage::GetUserKeyById(UserId uUserId)
{
	UserKey uKey = 0;
	m_lckIdKey.lock();
	auto iter = m_mapIdKey.find(uUserId);
	if (m_mapIdKey.end() != iter)
	{
		uKey = iter->second;
	}
	m_lckIdKey.unlock();
	return uKey;
}

MLock* UserInfoManage::GetClientUserLock(UserKey uUserKey)
{
	return &m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT];
}

void UserInfoManage::SetUserInfo(UserKey uUserKey, ClientUserInfo &info)
{
	ClientUserInfo *pUser = GetClientUserInfo(uUserKey);
	if (nullptr == pUser)
	{
		loge() << "�����ڵ�ǰ�û���";
		return;
	}

	MAutoLock lck(GetClientUserLock(uUserKey));
	pUser->m_strName = info.m_strName;
	pUser->m_strPassword = info.m_strPassword;


}