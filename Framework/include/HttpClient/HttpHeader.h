#pragma once
#include <WinSock2.h>
#include <string>
#include <xhash>
#include <unordered_map>
#include <map>

using namespace std;

//http��������
enum HTTP_METHOD
{
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_PATCH,
	HTTP_METHOD_GET,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_OPTIONS,
	HTTP_METHOD_CONNECT,
};

//Request HeadersԼ��
#define  HEADER_ACCEPT           "Accept"           //������ɽ��ܵ�MIME����
#define  HEADER_ACCEPT_ENCODING  "Accept-Encoding"  //���ݱ��뷽ʽ
#define  HEADER_ACCEPT_LANGUAGE  "Accept-Language"  //��������
#define  HEADER_AUTHORIZATION    "Authorization"    //ǩ��
#define  HEADER_SIGN_DATE        "SignDate"       //��Ϣ�����ʱ���
#define  HEADER_HOST             "Host"             //URL�е������Ͷ˿�
#define  HEADER_CONTENT_LENGTH   "Content-Length"   //��Ϣ���ĵĳ���
#define  HEADER_CONTENT_TYPE     "Content-Type"     //˵���������������������α���
#define  HEADER_MSG_IDENT        "MsgIdent"        //��ϢΨһ��ʶ
#define  HEADER_OPERATOR         "Operator"         //�ӿڵ�����uId

//value
#define  HEADER_ACCEPT_VALUE           "*/*"
#define  HEADER_ACCEPT_ENCODING_VALUE  "deflate"//"gzip, deflate"
#define  HEADER_ACCEPT_LANGUAGE_VALUE  "zh-CN"
#define  HEADER_CONTENT_TYPE_VALUE     "application/json"

#define  TAB_ITEM      "\n"

#define HTTP_METHOD_POST_STR					"POST"
#define HTTP_METHOD_PUT_STR						"PUT"
#define HTTP_METHOD_PATCH_STR					"PATCH"
#define HTTP_METHOD_GET_STR					    "GET"
#define HTTP_METHOD_DELETE_STR					"DELETE"
#define HTTP_METHOD_HEAD_STR					"HEAD"
#define HTTP_METHOD_TRACE_STR					"TRACE"
#define HTTP_METHOD_OPTIONS_STR					"OPTIONS"
#define HTTP_METHOD_CONNECT_STR					"CONNECT"


bool GetMacByGetAdaptersInfo(std::string& macOUT);
bool GetMacByGetAdaptersAddresses(std::string& macOUT);

class http_request_header
{
public:
	http_request_header(HTTP_METHOD method, unsigned int content_length, const string& access_key_id, const string& access_key_secret, const string& user_id);
	~http_request_header() {};
public:
	void auto_build_header();
	std::map<std::string, std::string>& GetHttpHead();
	string get_verb(HTTP_METHOD method);
	//�����
	void add_header_item(string name, string value);

private:
	//base64(hmac-sha1(AccessKeySecret,VERB  + "\n" + Content-Type + "\n" + SignDate+��\n��+ msg_ident)
	string make_authorization();
	//guid����:%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X
	string make_guid();
	string make_sign_date();
	string make_content_length(unsigned int content_length);
private:
	std::map<std::string, std::string>	m_headers;	//httpͷ�б�
	string m_verb;               //��ʾ��������:POST��GET�ȵ�
	string m_content_type;       //˵���������������������α���
	string m_accept;             //MIME����
	string m_accept_encoding;    //���ݱ��뷽ʽ
	string m_accept_language;    //��������
	string m_msg_ident;		     //��ϢΨһ��ʶ
	string m_sign_date;			 //��Ϣ�����ʱ���
	string m_access_key_secret;  //��ԿID
	string m_access_key_id;      //��Կ����
	string m_authorization;      //����ǩ��
	string m_content_length;     //�����峤��
    string m_user_id;            //�ӿڵ�����uId
};