#pragma once

class CXMLReader
{
public:

	CXMLReader();

	~CXMLReader();

public:

	//��ʼ��
	BOOL Init();

	//��XML�ļ�
	DWORD OpenXMLFile(LPCTSTR lpFileName);

	//�ر��ļ�
	void CloseXMLFile();

	//�Ҹ��ڵ�
	BOOL FindRoot();

	//��ȡ���ڵ��ǩ����
	BOOL GetTagName(CString& strName);

	//��ȡ��ǰ�ڵ����ӽڵ����
	LONG GetSubNodeNumber();

	//�������������ӽڵ�
	BOOL EnterSubNode(long lIndex);

	//���ݽڵ��ǩ�����������ӽڵ�
	BOOL EnterSubNode(LPCTSTR lpTagName, long lIndex = 0);

	//�˳��ӽڵ㣬���ص����ڵ���
	BOOL ExitSubNode();

	//�����������ƻ�ȡ��Ӧ������ֵ, int�汾
	BOOL GetAttributeValue(LPCTSTR lpAttr, int& iValue);

	//�����������ƻ�ȡ��Ӧ������ֵ, long�汾
	BOOL GetAttributeValue(LPCTSTR lpAttr, long& lValue);

	//�����������ƻ�ȡ��Ӧ������ֵ,CString�汾
	BOOL GetAttributeValue(LPCTSTR lpAttr, CString& strValue);

	//��ȡֵ
	BOOL GetText(CString& strValue);

private:

	CXMLReader(const CXMLReader& obj);

	CXMLReader& operator=(const CXMLReader& obj);

private:

	IXMLDocument*				m_pIXMLDoc;		//!< XML�ļ��ӿڶ���	

	IXMLElement*				m_pIXMLCurEmt;	//!< XML��ǰ�ڵ�Ԫ�ؽӿڶ���
};