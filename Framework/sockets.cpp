#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif

#include "sockets.h"


#if defined(_MSC_VER)
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE (1)
#endif
#pragma warning(disable : 4996)
#endif


//*****************************************************************************
//	����˵����	����s��ȡsocket��
//	����ԭ�ͣ�	int get_sock_name(SOCKET s, sockaddr_in *sock)
//  ������      s��ѯ��socket�����sock��ȡ������
//  ����ֵ��    len
//  �÷���		
//*****************************************************************************
int sockets::get_sock_name(sock_fd s, sockaddr_in *sock)
{
	sockaddr sockdata;
	int len = sizeof(sockdata);
	len = getsockname(s, &sockdata, &len);
	memcpy(sock, &sockdata.sa_data, sizeof(sockdata.sa_data));
	return len;
}


//*****************************************************************************
//	����˵����	TCP���ӳ�ʼ��
//	����ԭ�ͣ�	SOCKET tcp_link(char * ip, int port)
//  ������      ip��ַ��port�˿�
//  ����ֵ��    �ɹ�����socket���ֵ�����򷵻�0
//  �÷���		
//*****************************************************************************
sock_fd sockets::tcp_link(const std::string &ip, int port)
{
	struct	sockaddr_in	sock;
	struct	hostent	*host = 0;

	sock_fd hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	if (connect(hsocket, (struct sockaddr *)&sock, sizeof(sock)) != SOCKET_ERROR)
		return hsocket;
	closesocket(hsocket);
	return 0;
}

//*****************************************************************************
//	����˵����	TCP������ʼ��
//	����ԭ�ͣ�	SOCKET tcp_listen(u_short port)
//  ������      port�˿�
//  ����ֵ��    ��ȷhsocket�� ����0
//  �÷���		ͬ��
//*****************************************************************************
sock_fd sockets::tcp_listen(const std::string &ip, u_short port)
{
	sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket)
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
	if (!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// �󶨵�ַ�Ͷ˿�
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock)))
	{
		closesocket(hsocket);
		return 0;
	}
	// ��ʼ���м���
	if (SOCKET_ERROR == listen(hsocket, SOMAXCONN))
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}

//*****************************************************************************
//	����˵����	UDP���ӳ�ʼ��
//	����ԭ�ͣ�	SOCKET udp_link(char * ip, int port, sockaddr_in &sock)
//  ������      ip��ַ��port�˿ڡ�sockΪSOCKADDR_IN�ṹ
//  ����ֵ��    ͬ��
//  �÷���		ͬ��
//*****************************************************************************
sock_fd sockets::udp_link(const std::string &ip, int port, sockaddr_in &sock)
{
	struct hostent *host = 0;

	sock_fd hsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (hsocket == INVALID_SOCKET) return 0;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr.s_addr = inet_addr(ip.c_str());
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	if (sock.sin_addr.s_addr == INADDR_NONE && (host = gethostbyname(ip.c_str())))
		memcpy(&sock.sin_addr, host->h_addr_list[0], host->h_length);

	return hsocket;
}

//*****************************************************************************
//	����˵����	UDP������ʼ��
//	����ԭ�ͣ�	SOCKET udp_listen(u_short port)
//  ������      port�˿�
//  ����ֵ��    ��ȷhsocket�� ����0
//  �÷���		ͬ��
//*****************************************************************************
sock_fd sockets::udp_listen(const std::string &ip, u_short port)
{
	sock_fd hsocket;
	struct sockaddr_in sock;
	struct hostent *host = 0;
	hsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == hsocket)
	{
		return 0;
	}
	ZeroMemory((char *)&sock, sizeof(sock));
	sock.sin_family = AF_INET;
	if (!ip.empty())
		sock.sin_addr.s_addr = inet_addr(ip.c_str());
	else
		sock.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.sin_port = htons(port);
	// �󶨵�ַ�Ͷ˿�
	if (SOCKET_ERROR == bind(hsocket, (struct sockaddr *) &sock, sizeof(sock)))
	{
		closesocket(hsocket);
		return 0;
	}
	return hsocket;
}


//*****************************************************************************
//	����˵����	IP��ַת������DWORDתΪ�����ַ�������
//	����ԭ�ͣ�	const char* inet_ntoa(unsigned long ip_addr)
//  ������      ��ip_addr��DWORD���͵�IP��ַ
//  ����ֵ��    ��ȷIP��ַ�� ����XXX.XXX.XXX.XXX
//  �÷���		
//*****************************************************************************
const char* sockets::inet_ntoa(unsigned long ip_addr)
{
	static char* lpErrorAddr = "XXX.XXX.XXX.XXX";

	//ʹ��ϵͳ����ת��
	char*	lpAddress = ::inet_ntoa(*(in_addr*)(&ip_addr));
	if (NULL == lpAddress)
	{
		return lpErrorAddr;
	}
	return lpAddress;
}

//*****************************************************************************
//	����˵����	����length��������
//	����ԭ�ͣ�	int recv_to_end(sock_fd s,char *buffer,int length,int flag)
//  ������      ��recvһ��
//  ����ֵ��    ���������length�����ݣ�����length���������
//  �÷���		��recvһ��
//*****************************************************************************
int sockets::recv_to_end(sock_fd s, char *buffer, int length, int flag)
{
	char* ptr = buffer;
	int byteLeft = length;
	int l = 0;
	do {
		l = recv(s, ptr, byteLeft, flag);
		if (l == 0)
		{
			break;
		}
		ptr += l;
		byteLeft -= l;
	} while (byteLeft > 0 && l > 0);
	return length - byteLeft;
}

//*****************************************************************************
//	����˵����	����length��������
//	����ԭ�ͣ�	int send_to_end(sock_fd s,char *buffer,int length,int flag)
//  ������      ��recvһ��
//  ����ֵ��    ���������length�����ݣ�����length���������
//  �÷���		���Ϸ���һ��
//*****************************************************************************
int sockets::send_to_end(sock_fd s, char *buffer, int length, int flag)
{	// �ܹ���Ҫ���͵ģ��Ѿ����͵ĳ���
	int total_bytes = length, bytes_sent = 0;
	do
	{
		int send_bytes = total_bytes - bytes_sent;
		int ret_bytes = send(s, buffer + bytes_sent, send_bytes, flag);
		if (ret_bytes < 0)
			break;
		bytes_sent += ret_bytes;
	} while (bytes_sent != total_bytes);
	return bytes_sent;
}
