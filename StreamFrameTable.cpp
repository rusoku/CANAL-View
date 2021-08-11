#include "StreamFrameTable.h"
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QBrush>
#include <QSize>
#include <QDateTime>
#include <QChar>
#include <QLatin1Char>


RxFrameTable::RxFrameTable(QObject *parent/*, QVector<streamMsg>& rxmsglist*/) : QAbstractTableModel(parent), /*m_RxCanFrames(rxmsglist),*/ m_timer(new QTimer(this))
{
    //m_timer->setInterval(100);
    //connect(m_timer, &QTimer::timeout , this, &RxFrameTable::timerHit);
    //m_timer->start();
    m_StreamCanFrames = new QVector<streamMsg>;

}

RxFrameTable::~RxFrameTable()
{
    m_StreamCanFrames->clear();
    m_StreamCanFrames->squeeze();
    delete m_StreamCanFrames;
}

QVector<streamMsg>* RxFrameTable::getStreamDatabasePointer(void)
{
    return m_StreamCanFrames;
}

/* QTableItemModel */
QVariant RxFrameTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
           return QString("Timestamp (us)");
        case 1:
           return QString("T h:m:sec.ms");
        case 2:
          return  QString("Id type");
        case 3:
           return QString("Id (hex)");
        case 4:
            return QString("Length");
        case 5:
            return QString("Data (hex)");
        case 6:
            return QString("Ascii");
        }
    }

    if (role == Qt::SizeHintRole && orientation == Qt::Horizontal){

        //QSize size(100,10);
        //size.shrunkBy()
        //return size;
    }

    if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setBold(true);
        return font;
    }


//    if (role == Qt::BackgroundRole && orientation == Qt::Horizontal){
        //return QBrush(Qt::red);
//        return ???
//    }

    /*
        if (role == Qt::DisplayRole && orientation == Qt::Vertical)
        {
            //return QString("%1").arg(section + 1);
            return QVariant(section).toString();
        }
    */

    return QVariant();
}


int RxFrameTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
/*
    if(m_RxCanFrames->size() == 0)
        return 1;
    else
        return m_RxCanFrames->size();
*/
    return m_StreamCanFrames->size();
}

int RxFrameTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 7;
}

QVariant RxFrameTable::data(const QModelIndex &index, int role) const
{

    int row = index.row();
    int col = index.column();    

    streamMsg  canframe;

    QByteArray data_array;
    QString data;
    QChar ch;

/********************* Qt::DisplayRole  ********************/

    if (role == Qt::DisplayRole)
    {
        if(row < m_StreamCanFrames->size())
        {
           canframe = m_StreamCanFrames->at(row);

           switch(col)
           {
             case 0:
                 return QString("%1").arg(canframe.msg.timestamp,0,10);
             case 1:
                 return QDateTime::fromMSecsSinceEpoch(canframe.stat.time, Qt::UTC).toString("hh:mm:ss.zzz");
             case 2:
                {
                 QString  flag_string;

                 if(canframe.msg.flags & CANAL_IDFLAG_EXTENDED)
                     flag_string = "Ext";
                 else
                     flag_string = "Std";

                 if(canframe.msg.flags & CANAL_IDFLAG_RTR)
                     flag_string.append(":Rtr");

                 if(canframe.msg.flags & CANAL_IDFLAG_STATUS)
                     flag_string = "Status";

                 return flag_string;
                }
             case 3:
                   return QString().asprintf("%08X",canframe.msg.id);
             case 4:
                 return QString("%1").arg(canframe.msg.sizeData,0,10);
             case 5:
                {
                 data_array = QByteArray::fromRawData((const char*) canframe.msg.data, canframe.msg.sizeData & 0x0F).toHex(' ').toUpper();
                 //data = data_array.toHex(' ');
                 //return data.toUpper();
                 return data_array;
                }
             case 6:
                {
                 data_array = QByteArray::fromRawData((const char*) canframe.msg.data, canframe.msg.sizeData & 0x0F);

                   for(int x=0; x< canframe.msg.sizeData ; x++)
                   {
                    ch = data_array.at(x);
                    if(ch.isLetterOrNumber())
                    {
                     data.append(ch.toLatin1());
                    }
                     else
                        data.append('.');
                   }
                  return data;
               }
           }
        }
     }

/****************** Qt::TextAlignmentRole  *****************/

        if( role == Qt::ForegroundRole)
        {
            if(col == 2)
            {
             //return QColor(Qt::red);
            }
        }

/****************** Qt::TextAlignmentRole  *****************/

       if( role == Qt::TextAlignmentRole)
       {
           if (col != 5) //change text alignment only for cell(1,1)
           return Qt::AlignHCenter + Qt::AlignVCenter;
       }

       /****************** Qt::SizeHintRole  *****************/

       if( role == Qt::SizeHintRole )
       {
           if (col == 4) //change text alignment only for cell(1,1)
           {
            //QSize size;
            //size.setWidth(50);
            //return size;
           }
       }

    return QVariant();
}

Qt::ItemFlags RxFrameTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags;

    flags = QAbstractItemModel::flags(index);

            flags |=  Qt::ItemIsEditable;
            return flags;
}

void RxFrameTable::updateFrame(quint32 cnt, canalMsg rxmsg)
{
    Q_UNUSED(cnt);
    streamMsg  stream_msg;

//    ui->label_RxFramesCnt->setText(QString::number(cnt));
    //ui->progressBar->setValue(cnt);
    //RxTableModel->m_RxCanFrames.indexOf(rxmsg.id,0);

    m_FramesCnt = cnt;

    stream_msg.msg = rxmsg;
    stream_msg.stat.time = QDateTime::currentMSecsSinceEpoch();

    m_StreamCanFrames->append(stream_msg);
    if(updateFrameTimerEnable == false)
    {
      QTimer::singleShot(100, this ,SLOT(updateFrameTimerCallback()));
      updateFrameTimerEnable = true;
    }

    //ui->RxMsgTableView->scrollToBottom();
    //ui->RxMsgTableView->selectRow(RxTableModel->m_RxCanFrames.size()-1);
    //ui->RxMsgTableView->setFocus();
}

void RxFrameTable::updateFrameTimerCallback(void)
{
    //QModelIndex Left = createIndex(m_StreamCanFrames->size(),0);
    //QModelIndex Right = createIndex(m_StreamCanFrames->size(),6);

    //const QModelIndex idx = index(0);
    //emit dataChanged(idx ,idx );

    /* signal */
//    dataChanged(Left, Right);
    layoutAboutToBeChanged();
    layoutChanged();

    /* signal */
    emit newFrameArrived(m_FramesCnt);

    updateFrameTimerEnable = false;
}

/**************************** Timer *****************************/
void RxFrameTable::timerHit()
{
    //selectR
    //ui->RxMsgTableView->scrollToBottom();
    //ui->RxMsgTableView->selectRow(RxTableModel->m_RxCanFrames.size()-1);
    //ui->RxMsgTableView->setFocus();
}

