#include "ModelManage.h"
#include "../../CommonFile/CommonDefine.h"



ModelManage::ModelManage(IMainClient *_main)
	:IModelManage(_main)
{
}


ModelManage::~ModelManage()
{
}

bool ModelManage::Start()
{
	return true;
}

void ModelManage::Stop()
{
}

int ModelManage::SaveNumberList(QList<QList<QVariant> > rows)
{
    if (0 == rows.size())
    {
        loge() << QStringLiteral("��������ʧ�ܣ�����Ϊ0��");
        return 0;
    }
    rows.pop_front();		//ȥ��������

    m_mapNumberList.clear();
    for (QList<QVariant> row : rows)
    {
        QString qstrPeriod = row.at(1).toString();
        QString qstrNum = row.at(2).toString();
        m_mapNumberList[qstrPeriod] = qstrNum;
    }
    return m_mapNumberList.count();
}

int ModelManage::AddNumberData(QString qstrPeriod, QString qstrNum)
{
    if (qstrPeriod.isEmpty() || qstrNum.isEmpty())
    {
        loge()<<QString::fromLocal8Bit("��Ӻ�������ʧ��! period:%1 number:%2").arg(qstrPeriod).arg(qstrNum);
        return -1;
    }

    int icount = qstrNum.split(',').size();
    if(10 != icount)
    {
        loge()<<QString::fromLocal8Bit("��ӵĺ������󣬸�����Ϊ10��number:%1  ����:%2").arg(qstrNum).arg(icount);
        return -1;
    }

    m_mapNumberList[qstrPeriod] = qstrNum;
    return m_mapNumberList.count();
}

QList<QStringList> ModelManage::ExecuteStatistic(int iStatisticCount, int iStatisticFigure, QVector<int> rank)
{
    QList<QStringList> rows;
    int iCount = m_mapNumberList.size();
    if (0 == iCount || iCount < iStatisticCount)
    {
        loge() << QString::fromLocal8Bit("ͳ��ʧ�ܣ�������Ϊ:%1 ͳ������Ϊ:%2").arg(iCount).arg(iStatisticCount);
        return rows;
    }
   ::memset(m_aHitCount,0,sizeof(m_aHitCount));

    double dAvg;
   int iRow = 0;
   auto iter = m_mapNumberList.begin();
   for (iter = iter + (iCount - iStatisticCount); iter != m_mapNumberList.end(); iter++)
   {
       QStringList row;
       row.append(iter.key());
       //��Ӻ���
       QString qstrNums = iter.value();
       row.append(qstrNums);

        //���ƽ��ֵ
       dAvg = (iRow + 1) * iStatisticFigure / 10;
       row.append(QString::number(dAvg, 'f' ,1));

       //��Ӹ������ֵ��ȶ�
       StatisticOneHeat(qstrNums, rank);
       iCount = rank.size();
       for (int i = 0; i < iCount; i++)
       {
           row.append(QString::number(m_aHitCount[i]));
       }
       rows.append(row);
   }
    return rows;
}

const QMap<QString, QString> ModelManage::GetNumberList()
{
    return m_mapNumberList;
}

void ModelManage::StatisticOneHeat(QString &qstrNums, QVector<int> rank)
{
    int idx = 0;
    QStringList numList = qstrNums.split(',');
    for (QString qstrNum : numList)
    {
        int iNum = qstrNum.toInt() - 1;
        if (1 == rank[idx++])
        {
            m_aHitCount[iNum]++;
        }
    }
}
