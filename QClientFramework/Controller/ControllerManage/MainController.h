#pragma once
/************************************************************************
* filename:MainController.h
* function:�������������ڳ��������Լ��û���½ҵ��Ĵ���
* author :	������
* datetime:2018-12-14
* company:
/************************************************************************/

#include <QThread>
#include "ControllerColleague.h"

class MainModel;

class MainController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit MainController(ControllerManage*);
    ~MainController() override;

    bool Start();
	void Stop();

	void UserLoginResult(unsigned uUserKey, bool bRet, std::string strMsg);

signals:
	/*************************����Ϊ�����֪ͨ��Ϣ*************************/
	//��½��ʾ��Ϣ
	void signalLoginTipMsg(std::string strMsg);
	//��½��Ϣ֪ͨ
	void signalLoginMessageNt(bool bSuccess, std::string strMsg);
	//����������Ϣ
	void signalSplashMessage(std::string strMsg);
	//֪ͨ��ʾ������
	void signalShowMainWindow(unsigned uUserType);

	//������ʾ��Ϣ
	void signalPopupShowMessage(unsigned uType, std::string strTitle, std::string strMsg);

signals:
	/************************����Ϊ�ⲿ�첽ִ������************************/
	//�û���½
	void signalUserLogin(std::string strUserName, std::string strPassword);
	//ִ��ϵͳ
	void signalExecuteSystem();

private slots:
	/********************����Ϊ�ⲿ�첽ִ�������ʵ�ֲ�********************/
	//�û���½
	void slotUserLogin(std::string strUserName, std::string strPassword);

	//ִ��ϵͳ
	void slotExecuteSystem();


	/*************************����Ϊ�����Զ��ۺ���*************************/
	/*************************************************************************
	* function��		 Tcp����״̬֪ͨ
	* param uServerType: ����������
	* param state:		 ���ӳɹ���ʶ
	* param once:		 ��һ�����ӱ�ʶ
	*************************************************************************/
	void slotTcpConnectNotify(unsigned uServerType, bool bSuccess);
	void slotTcpDisconnectNotify(unsigned uServerType);

private:
    MainModel		*m_pMainModel = nullptr;
    QThread			m_thread;                   //�첽�߳�
};

