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

bool UserInfoManage::Initialize()
{
	if (nullptr == m_pMgr)
	{
		loge() << "m_pMgr Ϊ��";
		return false;
	}

	return true;
}

void UserInfoManage::Uninitialize()
{
	//m_mapIdKey.clear();				//�û�id��key��ӳ���ϵ
	//m_mapUserList.clear();			//�û�key��UserInfo��ӳ��
}

void UserInfoManage::AddUser(UserKey uUserKey)
{
	logm() << "�����ӵ���userKey:" << uUserKey;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (iter != m_mapUserList.end() && iter->second)
	{
		m_lckUserList.unlock();
		loge() << "�Ѿ����ڵ�ǰ�û�key:" << uUserKey <<"id:"<< iter->second->uUserId << "������ӣ�";
		return;
	}

	SUserInfo *pUser = m_rscUser.get();
	pUser->Reset();
	pUser->uUserKey = uUserKey;
	m_mapUserList[uUserKey] = pUser;
	m_lckUserList.unlock();

}

void UserInfoManage::DeleteUser(UserKey uUserKey)
{
	logm() << "���ӶϿ�userKey:" << uUserKey;
	SUserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() != iter)
	{
		pUser = iter->second;
		m_mapUserList.erase(uUserKey);
	}
	m_lckUserList.unlock();

	if (pUser)
	{
		m_lckIdKey.lock();
		m_mapIdKey.erase(pUser->uUserId);
		m_lckIdKey.unlock();

		m_rscUser.put(pUser);
	}
}

SUserInfo* UserInfoManage::GetClientUserInfo(UserKey uUserKey)
{
	SUserInfo *pUser = nullptr;
	m_lckUserList.lock();
	auto iter = m_mapUserList.find(uUserKey);
	if (m_mapUserList.end() != iter)
	{
		pUser = iter->second;
	}
	m_lckUserList.unlock();

	return pUser;
}

SUserInfo* UserInfoManage::GetClientUserInfoById(UserId uUserId)
{
	SUserInfo* pUser = nullptr;
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
		pUser = static_cast<SUserInfo*>(iter1->second);
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
			<< "��UserKey:" << uUserKey << "���ʧ�ܣ�";
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

void UserInfoManage::LockUserInfo(UserKey uUserKey)
{
	m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT].lock();
}

void UserInfoManage::UnlockUserInfo(UserKey uUserKey)
{
	m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT].unlock();
}

std::vector<unsigned> UserInfoManage::GetOfflineUserList()
{
	std::vector<unsigned> userKeyList;
	
	m_lckUserList.lock();
	
	for (auto iter = m_mapUserList.begin(); iter != m_mapUserList.end(); iter++)
	{
		SUserInfo *pInfo = iter->second;
		if (pInfo->uHeartCount >= 2)
		{
			userKeyList.push_back(pInfo->uUserKey);
		}
	}
	m_lckUserList.unlock();

	int iCount = userKeyList.size();
	if (iCount > 0) 
		LOGM("��ǰ���ߵ��У�%d��, ���ߵ��У�%d�ˡ�", iCount, m_mapUserList.size());

	return userKeyList;
}
