#pragma once

#define MESSAGE_RESOURCE_COUNT 100

//���������ͣ��ͻ��˵�UserKey��ʹ�ô�ö�ٱ���
enum EServerType
{
	EST_UNKNOWN,
	EST_CMD_SERVER,
	EST_DATA_SERVER,
	EST_SERVER_COUNT = 1000
};

//��ʾ��Ϣ����
enum EPopupMsgType
{
	EPMT_UNKNOWN,
	EPMT_INFORM,			//��Ϣ
	EPMT_WARNING,           //�澯
	EPMT_CRITICAL,          //����
	EPMT_QUESTION			//��ѯ
};
