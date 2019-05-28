#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "../../3rdParty/Framework/include/ResourceManage.h"
#include "..\..\CommonFile\TypeDefine.h"
#include "..\..\3rdParty\Framework\include\MLock.h"

class MainModel
{
public:
    MainModel();

    bool Start();
	void Stop();

	//�·����û��ɹ�����֪ͨ
	void AddUser(unsigned uUserKey);

	// ɾ���û�
	void DeleteUser(unsigned uUserKey);

private:
	mqw::ResourceManage<SUserInfo>	m_rscUser;
	//MLock				m_shareLock[CONNECT_SHARE_LOCK_COUNT];

	std::map<UserKey, SUserInfo*>	m_mapUserList;		//�û�key��UserInfo��ӳ��
	MLock							m_lckUserList;		//�û��б���
};

#endif // MAINMODEL_H
