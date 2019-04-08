#ifndef FORMSTATISTIC_H
#define FORMSTATISTIC_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include "../ViewManage/ViewColleague.h"

namespace Ui {
class FormStatistic;
}


class FormStatistic : public QWidget, public ViewColleague
{
    Q_OBJECT

public:
    explicit FormStatistic(ViewMediator *mdt, QWidget *parent = nullptr);
    ~FormStatistic();

signals:
    void signalBackToTab(QWidget*, QString qstrTitle);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_sbTatisticPeriod_editingFinished();

    void on_cbStatisticMode_currentIndexChanged(int index);

    void slotGroupButtonToggled(int id, bool checked);

    void slotNumberListChanged(int iCount);
    void slotStatisticResultNotify(QList<QStringList> rows);

private:
    void HighlightFifthRow();

private:
    Ui::FormStatistic *ui;
    int                           m_iStatisticCount;            //ͳ������
    QVector<int>            m_vctStatisticRank;     //��Ҫͳ�Ƶ�����
    int							  m_iStatisticFigure;	      //ͳ��λ��
};

#endif // FORMSTATISTIC_H
