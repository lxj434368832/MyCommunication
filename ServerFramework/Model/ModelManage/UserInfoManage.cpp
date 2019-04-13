#include "UserInfoManage.h"
#include "IModelManage.h"
#include "..\..\MainServer\IMainServer.h"
#include "..\..\Component\MessageHandle\IMessageBusiness.h"
#include "..\..\CommonFile\CommonDefine.h"

UserInfoManage::UserInfoManage(IModelManage* pMgr)
	:m_rscUser(USER_RESOURCE_COUNT)
{
	m_pMgr = pMgr;
	m_pMsgHandle = nullptr;
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
	if (nullptr == m_pMgr->GetMainServer())
	{
		loge() << "IMainServer Ϊ��";
		return false;
	}
	if (nullptr == m_pMgr->GetMainServer()->GetMessageBusiness())
	{
		loge() << "IMessageBusiness Ϊ��";
		return false;
	}

	m_pMsgHandle = m_pMgr->GetMainServer()->GetMessageBusiness();

	if (nullptr == m_pMsgHandle)
	{
		loge() << "IProtobufMsgFactory Ϊ��";
		return false;
	}

	return true;
}

void UserInfoManage::Stop()
{
	m_pMsgHandle = nullptr;			//���ٴ�����Ϣ
	m_funSendData = nullptr;		//�������ݻص�
	m_fuDisconnect = nullptr;		//�����Ͽ����ӻص�
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
		loge() << "�Ѿ����ڵ�ǰ�û�key:" << uUserKey <<"id:"<< iter->second->m_uUserId << "������ӣ�";
		return;
	}

	ClientUserInfo *pUser = m_rscUser.get();
	pUser->Reset();
	pUser->m_uUserKey = uUserKey;
	m_mapUserList[uUserKey] = pUser;
	m_lckUserList.unlock();

	logm() << "�����ӵ���userKey:" << uUserKey;
}

void UserInfoManage::HandData(UserKey uUserKey, unsigned uMsgType, const char* data, unsigned length)
{
	uMsgType;
	if (m_pMsgHandle)
	{
		m_pMsgHandle->HandleProtobufMessage(uUserKey, data, length);
	}
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

void UserInfoManage::SendData(UserKey uUserKey, const char * data, unsigned uLength)
{
	if (m_funSendData)
		m_funSendData(uUserKey, 0, data, uLength);
}

void UserInfoManage::Disconnect(UserKey uUserKey)
{
	if (m_fuDisconnect)
	{
		m_fuDisconnect(uUserKey);
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

MLock* UserInfoManage::GetClientUserLock(UserKey uUserKey)
{
	return &m_UserShareLock[uUserKey % USER_SHARE_LOCK_COUNT];
}
