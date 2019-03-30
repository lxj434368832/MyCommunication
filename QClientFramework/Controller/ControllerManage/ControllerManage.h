#pragma once
#include <QObject>
#include <QThread>
#include <QMap>
#include "IControllerManage.h"

class ControllerManage : public QObject, public IControllerManage
{
    Q_OBJECT
public:
    ControllerManage(IMainClient *_main = nullptr);
    ~ControllerManage() override;

	bool Start() override;
	void Stop() override;

//�ⲿ���õ��ź�
signals:
    /*************************************************************************
    * function:�ӱ����ļ��е�������
    * param qstrFilePath: �ļ�·��
    * return:
    *************************************************************************/
    void signalImportData(QString qstrFilePath);

//�ⲿ���õĺ���
public:
    /*************************************************************************
    * function:����һ�����ݵ��б���
    * param qstrPeriod:����
    * param qstrNum:   ������
    * return:
    *************************************************************************/
    void AddData(QString qstrPeriod, QString qstrNum);

    const QMap<QString, QString> GetLotteryList();

//֪ͨ���źź���
signals:
    void signalLotteryListChanged(QMap<QString, QString>);

private slots:
    void slotImportData(QString qstrFilePath);

private:
    QThread                                     m_thread;                   //�첽�߳�
    ModelManage                           *m_model;                  //ģ�͹���
};

