#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMap>
#include "ControllerColleague.h"

class StatisticModel;
class IMainClient;

class StatisticController : public ControllerColleague
{
    Q_OBJECT
public:
    explicit StatisticController(IMainClient *main);
    ~StatisticController( ) override;

    bool Start();
    void Stop();

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
        StatisticModel                           *m_statisticModel;      //ͳ��ģ��
        QThread                                     m_thread;                    //�첽�߳�
};

#endif // STATISTICCONTROLLER_H
