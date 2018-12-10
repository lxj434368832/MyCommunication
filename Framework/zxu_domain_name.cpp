#include "../../include/zxu_utils/zxu_domain_name.h"

int get_host_ip(char *host_name, ADDRESS_FAMILY af, std::list<std::wstring> &list);

int zxu::get_host_ipv4(char *host_name, std::list<std::wstring> &list)
{
	return get_host_ip(host_name, AF_INET, list);
}

int zxu::get_host_ipv6(char *host_name, std::list<std::wstring> &list)
{
	return get_host_ip(host_name, AF_INET6, list);
}

int get_host_ip(char *host_name, ADDRESS_FAMILY af, std::list<std::wstring> &list)
{
	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	DWORD dwRetval = 0;

	TCHAR ipstringbuffer[200];
	DWORD bufferLength = 200;

	list.clear();

	//--------------------------------
	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;
	hints.ai_protocol = 0;

	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
	dwRetval = getaddrinfo(host_name, NULL, &hints, &result);
	if (dwRetval != 0)
	{
		return dwRetval = WSAGetLastError();
	}

	// Retrieve each address and print out the hex bytes
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		dwRetval = WSAAddressToString(ptr->ai_addr, ptr->ai_addrlen, NULL, ipstringbuffer, &bufferLength);

		if (dwRetval)
		{
			dwRetval = WSAGetLastError();
			continue;
		}

		if (af == ptr->ai_family)
		{
			list.push_back(ipstringbuffer);
			dwRetval = 0;
		}
	}
	freeaddrinfo(result);
	dwRetval = list.size() ? 0 : 1;
	return dwRetval;
}

#ifdef _WINSOCK_DEPRECATED_NO_WARNINGS
/************************************************************************
*function:��������,ֻ�ܻ�ȡIPV4��ַ��ǰ����Ҫ�ȵ���WSAStartup(����ʼ��winsock
*			�����ʹ�õ��ϰ汾������ʹ������֧��ipv6�ķ�ʽ
*param host_name:���ַ�������������ʶ����www.baidu.com
*param add_list: ��ȡ����IP��ַ�б�һ��ֻ��һ��
*return bool��   �ɹ�����true��ʧ�ܷ���false
************************************************************************/
bool get_host_ip(char *host_name, std::list<std::string>& add_list)
{
	bool bRet = true;
	DWORD dwError = 0;
	int i = 0;
	hostent *remoteHost = NULL;
	in_addr addr;
	char **pAlias = NULL;

	if (NULL == host_name)
	{
		std::cout << "�ַ���Ϊ�գ����飡" << std::endl;
		return false;
	}

	remoteHost = gethostbyname(host_name);

	if (remoteHost == NULL)
	{
		dwError = WSAGetLastError();
		if (0 != dwError)
		{
			if (WSAHOST_NOT_FOUND == dwError)
			{
				std::cout << "����δ�ҵ���" << std::endl;
			}
			else if (dwError == WSANO_DATA)
			{
				std::cout << "��������ݴ������飡" << std::endl;
			}
			else
			{
				std::cout << "��ȡ����IPʧ��,�����룺" << dwError;
			}
		}
		std::cout << "������������" << std::endl;
		bRet = false;
	}
	else
	{
		std::cout << "��������" << remoteHost->h_name << std::endl;
		for (pAlias = remoteHost->h_aliases; *pAlias != NULL; pAlias++)
		{
			std::cout << "��ѡ��������#" << i << ": " << *pAlias << std::endl;
		}

		i = 0;
		if (AF_INET == remoteHost->h_addrtype)
		{
			while (remoteHost->h_addr_list[i] != 0)
			{
				addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
				add_list.push_back(inet_ntoa(addr));
			}
		}
		else
		{
			std::cout << "�����ĵ�ַ���ʹ���" << std::endl;
			bRet = false;
		}
	}

	return bRet;
}

#endif // _WINSOCK_DEPRECATED_NO_WARNINGS

