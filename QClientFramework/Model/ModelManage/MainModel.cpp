#include "MainModel.h"
#include "..\..\CommonFile\EnumDefine.h"
#include "..\..\CommonFile\CommonDefine.h"

MainModel::MainModel()
{

}

bool MainModel::Initialize()
{
    return true;
}

void MainModel::Uninitialize()
{

}

void MainModel::LockUserInfo()
{
	m_lckUserInfo.lock();
}

void MainModel::UnlockUserInfo()
{
	m_lckUserInfo.unlock();
}

void MainModel::AddLoginServer(UserKey uUserKey)
{
	m_lckSrvUser.lock();
	m_setSrvUser.insert(uUserKey);
	m_lckSrvUser.unlock();
}

void MainModel::DelLoginServer(UserKey uUserKey)
{
	m_lckSrvUser.lock();
	m_setSrvUser.remove(uUserKey);
	m_lckSrvUser.unlock();
}

QSet<unsigned> MainModel::GetLoginServerList()
{
	return m_setSrvUser;
}

SUserInfo& MainModel::GetUserInfo()
{
	return m_user;
}

void MainModel::SaveUserInfo(SUserInfo& user)
{
	m_user = user;
}
