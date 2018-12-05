#pragma once

#include <map>
#include "UserInfo.h"

class INetInterface
{
public:
	INetInterface();
	virtual ~INetInterface();

	//�·����û��ɹ�����֪ͨ
	virtual void AddUser(unsigned uUserKey);

	//�����������
	virtual void HandSrvData(unsigned uUserKey, const char* data, unsigned length);

	// ����ɹ�ɾ���û�����ɾ���ĵ�ַ,����û��������򷵻�NULL
	virtual void DeleteUser(unsigned uUserKey);

protected:
	std::map<unsigned, UserInfo> m_mapUserList;

};

