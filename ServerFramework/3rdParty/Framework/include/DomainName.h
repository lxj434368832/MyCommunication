#ifndef ZXU_DOMAIN_NAME_H_
#define ZXU_DOMAIN_NAME_H_
/************************************************************************
*function: ������ص��ļ�,Ŀǰֻ֧�ֽ�����������IP��ַ
*author��  ������
*company�� ambition tech
*datetime��2017-11-29
************************************************************************/

#include <list>
#include <ws2tcpip.h>

namespace zxu
{
	enum e_domain_name_code
	{
		ednc_no_error = 0,				//��ȡ�ɹ���û�д���
		ednc_af_error = 1,				//��ȡ�ĵ�ַ�������
		ednc_host_not_found = WSAHOST_NOT_FOUND,	//����δ�ҵ�
		ednc_host_name_error = WSANO_DATA			//�������ִ���
							//����Ϊͨ��WSAGetLastError()��ȡ�Ĵ�����
	};
	
	/************************************************************************
	*function:��������,ֻ�ܻ�ȡIPV4��ַ��ǰ����Ҫ�ȵ���WSAStartup(����ʼ��winsock
	*������Ҫʹ��InetPton��InetNtop������ص�ת��
	*param host_name:���ַ�������������ʶ����www.baidu.com
	*param add_list: ��ȡ����IP��ַ�б�һ��ֻ��һ��
	*return int��   ���庬��μ�eDomainNameCode
	************************************************************************/
	int get_host_ipv4(char *host_name, std::list<std::wstring> &list);

	/************************************************************************
	*function:��������,ֻ�ܻ�ȡIPV6��ַ��ǰ����Ҫ�ȵ���WSAStartup(����ʼ��winsock
	*������Ҫʹ��InetPton��InetNtop������ص�ת��
	*param host_name:���ַ�������������ʶ����www.baidu.com
	*param add_list: ��ȡ����IP��ַ�б�һ��ֻ��һ��
	*return int��   ���庬��μ�eDomainNameCode
	************************************************************************/
	int get_host_ipv6(char *host_name, std::list<std::wstring> &list);
}


/* �����ǵ���ʾ��,�����ο�
int main(int argc, char **argv)
{
WSADATA wsaData;
int iResult;

// �˴�Ӧ��ӵĳ�������
iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
if (iResult != 0)
{
std::cout << "��ʼ��WSAStartupʧ��!" << std::endl;
return 1;
}

std::list<std::wstring> strIP_list;
char *host_name = "www.cnambition.com";
std::cout << "������������ :" << host_name << std::endl;
int retval = zxu::get_host_ipv4(host_name, strIP_list);

switch (retval)
{
case zxu::ednc_no_error:
std::cout << "��ȡ����IP�ɹ����б�Ϊ��\n";
for each (auto strIP in strIP_list)
{
std::wcout << "IP address��" << strIP.c_str() << std::endl;
}
break;
case zxu::ednc_af_error:
std::cout << "��ȡ�ĵ�ַ�������!" << std::endl;
break;
case zxu::ednc_host_not_found:
std::cout << "��ȡIP��ַ��������δ�ҵ���" << std::endl;
break;
case zxu::ednc_host_name_error:
std::cout << "��ȡIP��ַ��������������������" << std::endl;
break;
default:
std::cout << "��ȡIP��ַ���󣬴������ǣ�" << retval << std::endl;
break;
}

system("pause");
WSACleanup();
return 0;
}
*/

#endif
