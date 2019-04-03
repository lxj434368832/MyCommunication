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

    /*************************************************************************
    * function:�ӱ����ļ��е�������
    * param qstrFilePath: �ļ�·��
    * return:
    *************************************************************************/
    void signalExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

//�ⲿ���õĺ���
public:
    /*************************************************************************
    * function:����һ�����ݵ��б���
    * param qstrPeriod:����
    * param qstrNum:   ����
    * return:
    *************************************************************************/
    void AddData(QString qstrPeriod, QString qstrNum);

    const QMap<QString, QString> GetNumberList();

//֪ͨ���źź���
signals:
    void signalNumberListChanged(int iCount);
    void signalStatisticResultNotify(QList<QStringList> rows);

private slots:
    void slotImportData(QString qstrFilePath);
    void slotExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

private:
    QThread                                     m_thread;                   //�첽�߳�
    ModelManage                           *m_model;                  //ģ�͹���
};

