#pragma once

#include <wtypes.h>
#include <string>

class ConfigManage
{
public:
	ConfigManage(std::string strFileName);
	~ConfigManage();

	void SetValue(const std::string prefix, const std::string key, const std::string value);
	void SetDefault(const std::string prefix, const std::string key, const std::string value);
	std::string GetValue(const std::string prefix, const std::string key, const std::string defaultValue = std::string());

	void SetValueInt(const std::string prefix, const std::string key, const int value);
	void SetDefaultInt(const std::string prefix, const std::string key, const int value);
	int GetValueInt(const std::string prefix, const std::string key, const int defaultValue = -1);

private:

	// ע������
	HKEY								m_keyRoot;

	// �����ļ����
	std::string							m_strConfigFile;
};
