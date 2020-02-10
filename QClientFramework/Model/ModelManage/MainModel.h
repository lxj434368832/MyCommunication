#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"
#include <vector>


class MainModel
{
public:
    MainModel();

    bool Initialize();
	void Uninitialize();

	//�·����û��ɹ�����֪ͨ
	void AddServerUser(unsigned uUserKey);

	// ɾ�������û�
	void DeleteServerUser(unsigned uUserKey);

	SUserInfo* GetServerUser(unsigned uUserKey);

	/*************************************************************************
	* function����סUserKey��Ӧ����Ϣ
	*************************************************************************/
	void LockServerUserInfo(UserKey uUserKey);

	/*************************************************************************
	* function������UserKey��Ӧ����Ϣ
	*************************************************************************/
	void UnlockServerUserInfo(UserKey uUserKey);

	std::vector<unsigned> GetLoginUserList();

	SUserInfo& GetUserInfo();

	void SaveUserInfo(SUserInfo& user);

private:
	mqw::ResourceManage<SUserInfo>	m_rscUser;
	MLock				m_UserShareLock[USER_SHARE_LOCK_COUNT];

	std::map<UserKey, SUserInfo*>	m_mapUserList;		//�û�key��UserInfo��ӳ��
	MLock							m_lckUserList;		//�û��б���

	SUserInfo						m_user;
};

#endif // MAINMODEL_H
