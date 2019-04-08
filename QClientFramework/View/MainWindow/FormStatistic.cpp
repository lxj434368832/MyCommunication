#include "FormStatistic.h"
#include "ui_FormStatistic.h"
#include "../../Controller/ControllerManage/ControllerManage.h"
#include "../../Controller/ControllerManage/StatisticController.h"
#include "../../CommonFile/CommonDefine.h"
#include <QButtonGroup>
#include <QCloseEvent>
#include <QElapsedTimer>

FormStatistic::FormStatistic(ViewMediator *mdt, QWidget *parent) :
    QWidget(parent),
    ViewColleague (mdt),
    ui(new Ui::FormStatistic)
{
    ui->setupUi(this);

    int index = 0;
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(false);
    btnGroup->addButton(ui->cbrOne, index++);
    btnGroup->addButton(ui->cbrTwo, index++);
    btnGroup->addButton(ui->cbrThree, index++);
    btnGroup->addButton(ui->cbrFour, index++);
    btnGroup->addButton(ui->cbrFive, index++);
    btnGroup->addButton(ui->cbrSix, index++);
    btnGroup->addButton(ui->cbrSeven, index++);
    btnGroup->addButton(ui->cbrEight, index++);
    btnGroup->addButton(ui->cbrNine, index++);
    btnGroup->addButton(ui->cbrTen, index++);
    connect(btnGroup,SIGNAL(buttonToggled(int,bool)),this, SLOT(slotGroupButtonToggled(int,bool)));

    //��ʼ��ͳ���б���
    ui->twStatisticList->horizontalHeader()->setDefaultSectionSize(50);
    ui->twStatisticList->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    QStringList headers;
    headers.append(QString::fromLocal8Bit("�ں�"));
    headers.append(QString::fromLocal8Bit("����"));
    headers.append("AVG");
    headers.append("1");
    headers.append("2");
    headers.append("3");
    headers.append("4");
    headers.append("5");
    headers.append("6");
    headers.append("7");
    headers.append("8");
    headers.append("9");
    headers.append("10");
    ui->twStatisticList->setColumnCount(headers.count());
    ui->twStatisticList->setColumnWidth(0, 135);
    ui->twStatisticList->setColumnWidth(1, 280);
    ui->twStatisticList->setColumnWidth(2, 50);
    ui->twStatisticList->setHorizontalHeaderLabels(headers);

    connect(m_controller->GetStatisticController(), SIGNAL(signalNumberListChanged(int)),
            this, SLOT(slotNumberListChanged(int)));
    connect(m_controller->GetStatisticController(), SIGNAL(signalStatisticResultNotify(QList<QStringList>)),
            this, SLOT(slotStatisticResultNotify(QList<QStringList>)));

    m_iStatisticCount = ui->sbTatisticPeriod->value();
    m_vctStatisticRank = {0,0,1,1,1,1,1,1,0,0};
    m_iStatisticFigure = 6;
}

FormStatistic::~FormStatistic()
{
    delete ui;
}

void FormStatistic::closeEvent(QCloseEvent *event)
{
    emit signalBackToTab(this, windowTitle());
    event->ignore();
}

void FormStatistic::on_sbTatisticPeriod_editingFinished()
{
    m_iStatisticCount = ui->sbTatisticPeriod->value();
    emit m_controller->GetStatisticController()->signalExecuteStatistic(m_iStatisticCount, m_iStatisticFigure, m_vctStatisticRank);
}

void FormStatistic::on_cbStatisticMode_currentIndexChanged(int index)
{
    index;
    emit m_controller->GetStatisticController()->signalExecuteStatistic(m_iStatisticCount, m_iStatisticFigure, m_vctStatisticRank);
}

void FormStatistic::slotGroupButtonToggled(int id, bool checked)
{
    if (checked)
    {
        m_vctStatisticRank[id] = 1;
        m_iStatisticFigure++;
    }
    else
    {
        m_vctStatisticRank[id] = 0;
        m_iStatisticFigure--;
    }
    emit m_controller->GetStatisticController()->signalExecuteStatistic(m_iStatisticCount, m_iStatisticFigure, m_vctStatisticRank);
}

void FormStatistic::slotNumberListChanged(int iCount)
{
    ui->leTotalPeriod->setText(QString::number(iCount));
    emit m_controller->GetStatisticController()->signalExecuteStatistic(m_iStatisticCount, m_iStatisticFigure, m_vctStatisticRank);
}

void FormStatistic::slotStatisticResultNotify(QList<QStringList> rows)
{
    int iRowCount = rows.size();
    ui->twStatisticList->setRowCount(iRowCount);
    QTableWidgetItem *item;
    int iRow = 0;
    for (QStringList row : rows)
    {
        int iCol = 0;
        for(QString qstrCol : row)
        {
            //����ں�
            item = new QTableWidgetItem(qstrCol);
            item->setTextAlignment(Qt::AlignCenter);
            ui->twStatisticList->setItem(iRow, iCol++, item);
        }
        iRow++;
    }
    HighlightFifthRow();
}

void FormStatistic::HighlightFifthRow()
{
    QTableWidgetItem *item = nullptr;
    int iRowCount = ui->twStatisticList->rowCount();
    int iColCount = ui->twStatisticList->colorCount();
    for (int iRow = 4; iRow < iRowCount; iRow+= 5)
    {
        for (int iCol = 0; iCol < iColCount; iCol++)
        {
            item = ui->twStatisticList->item(iRow, iCol);
            if(item)
                item->setBackground(QColor("#deffde"));//#fda4a4
        }
    }
}
