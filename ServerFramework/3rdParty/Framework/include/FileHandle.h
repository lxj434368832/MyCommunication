/************************************************************************
* function:�ļ���������
* author :	������
* datetime:2018-04-03
* company:  ���̽ݿƼ��ɷ����޹�˾
/************************************************************************/
#pragma once
#include <wtypes.h>
#include <string>
#include <vector>
#include <list>

class FileHandle
{
public:
	FileHandle();
	~FileHandle();

	/*************************************************************************
	* function:			��ȡ�ļ��汾��
	* param FilePath:	in,�ļ���·��
	* return:			��ǰ�ļ���Ӧ�İ汾��
	*************************************************************************/
	static void GetFileVersion(LPCSTR FilePath, LPSTR buffer);

	/*************************************************************************
	* function:			��ȡ��Ŀ¼���Զ�ȥ��ĩβ��/��\,
	* param strPath:	in,��Ҫ������·����
	* return:			��ǰ·����Ӧ�ĸ�Ŀ¼
	*************************************************************************/
	static std::string GetParentDir(LPCSTR strPath);

	/*************************************************************************
	* function:			����Ŀ¼�������༶Ŀ¼
	* param strDir:		in,Ŀ¼��
	* return:			0��ʾ�ɹ�������Ϊ�������
	*************************************************************************/
	static int CreateDir(LPCSTR strDir);

	/*************************************************************************
	* function:			����������ʱ������Զ�ȥ��ĩβ��/��\,
	* param lpName:		in,��Ҫ������������
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static std::string RenameWithTimeStamp(LPCSTR lpName);

	/*************************************************************************
	* function:			���ļ���Ŀ¼������ʱ���
	* param lpName:		in,��Ҫ��ʱ���������
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static std::string AddTimeStapToName(LPCSTR lpName);

	/*************************************************************************
	* function:			�����ļ�������Ŀ¼��ʹ�õ��������еķ�ʽ
	* param lpSource:	in,��Ҫ������Դ�ļ���Դ�ļ���
	* param lpDest:		in,��Ҫ������Ŀ���ļ���Ŀ���ļ���
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static BOOL CopyFile(LPCTSTR lpSource, LPCTSTR lpDest);

	/*************************************************************************
	* function:			�����ļ���
	* param lpSource:	in,��Ҫ������Դ�ļ���
	* param lpDest:		in,��Ҫ������Ŀ���ļ���
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static bool CopyDirectory(LPCTSTR lpSource, LPCTSTR lpDest);

	/*************************************************************************
	* function:			ɾ���ļ�������Ŀ¼��ʹ�õ��������еķ�ʽ
	* param lpPath:		in,��Ҫɾ�����ļ����ļ���
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static BOOL DeleteFile(LPCTSTR lpPath);

	/*************************************************************************
	* function:			ɾ���ļ���
	* param lpSource:	in,��Ҫɾ����Դ�ļ���
	* return:			�ɹ�����true,ʧ�ܷ���false
	*************************************************************************/
	static bool DeleteDirectory(LPCTSTR lpSource);

	/*************************************************************************
	* function:			��ȡĿ¼�µ������ļ�
	* param lpDir:		in,Ŀ¼·��
	* param filter:		in,��������
	* return:			�ļ��б�
	*************************************************************************/
	static std::list<std::string> GetAllFile(LPCTSTR lpDir,LPCTSTR filter);
};
