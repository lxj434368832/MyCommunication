#include "MMainWindow.h"
#include "ui_MMainWindow.h"
#include "FormStatistic.h"
#include "..\..\Controller\ControllerManage\ControllerManage.h"
#include "formAddData.h"
#include "..\..\CommonFile\CommonDefine.h"
#include <QFileDialog>
#include <QElapsedTimer>

MMainWindow::MMainWindow(ViewMediator *mdt, QWidget *parent) :
    MFramelessWidget(parent),
    ViewColleague (mdt),
    ui(new Ui::MMainWindow)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    ui->frameStatusBar->setVisible(false);
    on_btnAddStatistic_clicked();
}

MMainWindow::~MMainWindow()
{
    delete ui;
}

void MMainWindow::on_btnClose_clicked()
{
    close();
}

void MMainWindow::on_btnImportData_clicked()
{
    QString qstrFilePath = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("��ѡ��Ҫ�����Excel�ļ�"),
		"", QString::fromLocal8Bit("Excel ������(*.xlsx *.xls)"));

	if (qstrFilePath.isEmpty()) return;
	if (false == QFile::exists(qstrFilePath)) return;
    emit m_mainLogic->signalImportData(QDir::toNativeSeparators(qstrFilePath));
}

void MMainWindow::on_btnAddData_clicked()
{
	QString qstrPeriod;
	QString qstrNum;
	formAddData	dlg;
	const QMap<QString, QString> lotteryList = m_mainLogic->GetLotteryList();
	if (false == lotteryList.isEmpty())
	{
		qstrPeriod = lotteryList.lastKey();
	}
    dlg.SetLastPeriod(qstrPeriod);
    if(QDialog::Rejected == dlg.exec()) return;
    dlg.GetData(qstrPeriod, qstrNum);
    m_mainLogic->AddData(qstrPeriod, qstrNum);
}

void MMainWindow::on_btnAddStatistic_clicked()
{
    int iItemCount = ui->tabStatistic->count();
    FormStatistic *form = new FormStatistic(m_mediator);
    connect(form, SIGNAL(signalBackToTab(QWidget*, QString)), this, SLOT(slotBackToTab(QWidget*, QString)));

    ui->tabStatistic->addTab(form, QString::fromLocal8Bit("ͳ����%1").arg(iItemCount + 1));
    ui->tabStatistic->setCurrentIndex(iItemCount);
}

void MMainWindow::on_tabStatistic_tabBarDoubleClicked(int index)
{
    QWidget *widget = ui->tabStatistic->widget(index);
    widget->setWindowTitle(ui->tabStatistic->tabText(index));
    ui->tabStatistic->removeTab(index);
    widget->setWindowFlags(Qt::Window);
    widget->show();
    widget->raise();
    widget->activateWindow();
	widget->move(50, 50);
}

void MMainWindow::slotBackToTab(QWidget* widget, QString qstrTitle)
{
    widget->setWindowFlags(Qt::Widget);
    ui->tabStatistic->addTab(widget, qstrTitle);
}

void MMainWindow::on_btnMinimum_clicked()
{
    showMinimized();
}

void MMainWindow::on_btnMaximum_clicked()
{
    if(isMaximized())
        showNormal();
    else
        showMaximized();
}
