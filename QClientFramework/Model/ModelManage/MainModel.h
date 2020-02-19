#pragma once

#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include <QSet>

class MainModel
{
public:
    MainModel();

    bool Initialize();
	void Uninitialize();

	void SaveUserInfo(SUserInfo& user);

	SUserInfo& GetUserInfo();

	/*************************************************************************
	* function����סUserKey��Ӧ����Ϣ
	*************************************************************************/
	void LockUserInfo();

	/*************************************************************************
	* function������UserKey��Ӧ����Ϣ
	*************************************************************************/
	void UnlockUserInfo();

	void AddLoginServer(UserKey uUserKey);

	QSet<unsigned> GetLoginServer();

private:
	SUserInfo	m_user;
	MLock		m_lckUserInfo;
	QSet<unsigned>	m_setSrvUser;
};
