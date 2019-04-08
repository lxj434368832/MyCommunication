#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H

#include <QMap>
#include <QList>
#include <QVariant>

class StatisticModel
{
public:
    StatisticModel();
    bool Start();
    void Stop();

    int SaveNumberList(QList<QList<QVariant>>);
    int AddNumberData(QString qstrPeriod, QString qstrNum);
    QList<QStringList> ExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

    const QMap<QString, QString> GetNumberList();

private:
    void StatisticOneHeat(QString &qstrNums, QVector<int> rank);

private:

    QMap<QString, QString>        m_mapNumberList;     //����������
    int                                                m_aHitCount[10];        //1--10���еĴ���
};

#endif // STATISTICMODEL_H
