#pragma once

#include <QString>

class NetworkHelp
{
public:
	NetworkHelp();
	~NetworkHelp();

	// ���ip��ַ  
	static QString GetIpAdress();
	// ����û���  
	static QString GetUserName();
	// ��û�������  
	static QString GetMachineName();
    // ���MAC��ַ
	static QString GetMACAdress();
};

