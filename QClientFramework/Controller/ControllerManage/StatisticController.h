#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMap>

class StatisticModel;

class StatisticController : public QObject
{
    Q_OBJECT
public:
    explicit StatisticController(QObject *parent = nullptr);
    ~StatisticController( ) override;

    bool Start();
    void Stop();

    //外部调用的信号
    signals:
        /*************************************************************************
        * function:从本地文件中导入数据
        * param qstrFilePath: 文件路径
        * return:
        *************************************************************************/
        void signalImportData(QString qstrFilePath);

        /*************************************************************************
        * function:从本地文件中导入数据
        * param qstrFilePath: 文件路径
        * return:
        *************************************************************************/
        void signalExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

    //外部调用的函数
    public:
        /*************************************************************************
        * function:增加一行数据到列表中
        * param qstrPeriod:期数
        * param qstrNum:   号码
        * return:
        *************************************************************************/
        void AddData(QString qstrPeriod, QString qstrNum);

        const QMap<QString, QString> GetNumberList();

    //通知的信号函数
    signals:
        void signalNumberListChanged(int iCount);
        void signalStatisticResultNotify(QList<QStringList> rows);

    private slots:
        void slotImportData(QString qstrFilePath);
        void slotExecuteStatistic(int iStatisticCount, int iStatisticFigure,QVector<int> rank);

    private:
        QThread                                     m_thread;                   //异步线程
        StatisticModel                           *m_statisticModel;      //模型
};

#endif // STATISTICCONTROLLER_H
