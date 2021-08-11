#include "RxFrameTable.h"
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QBrush>
#include <QSize>
#include <QDateTime>

//m_RxMsgList = new QVector<canalMsg>;

RxFrameTable::RxFrameTable(QObject *parent/*, QVector<canalMsg>& rxmsglist*/) : QAbstractTableModel(parent), /*m_RxCanFrames(rxmsglist),*/ m_timer(new QTimer(this))
{
    //m_timer->setInterval(100);
    //connect(m_timer, &QTimer::timeout , this, &RxFrameTable::timerHit);
    //m_timer->start();
    m_RxCanFrames = new QVector<canalMsg>;

}

RxFrameTable::~RxFrameTable()
{
    m_RxCanFrames->clear();
    m_RxCanFrames->squeeze();
    delete m_RxCanFrames;
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
          return  QString("Type");
        case 2:
           return QString("Id");
        case 3:
            return QString("Length");
        case 4:
            return QString("Data");
        }
    }


    if (role == Qt::SizeHintRole && orientation == Qt::Horizontal){

        //QSize size(100,10);
        //size.shrunkBy()
        //return size;
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

    return m_RxCanFrames->size();
    //return 1;
}

int RxFrameTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 5;
}

QVariant RxFrameTable::data(const QModelIndex &index, int role) const
{

    int row = index.row();
    int col = index.column();
 //    uint cnt = 0;

/*
    typedef struct _CANframe
    {
        quint32 timestamp;
        quint8  flags;
        quint32 id;
        quint8  sizeData;
        quint8  data[64];
    } CANframe;
*/
    //CANframe  canframeFD;
    canalMsg  canframe;

    //canframeFD.sizeData = 64;
    //for(int x=0; x<64; x++) canframeFD.data [x] = x;

/*
    typedef struct structCanalMsg {
      unsigned long flags;			    // CAN message flags
        unsigned long obid;			    	// Used by driver for channel info etc.
        unsigned long id;			      	// CAN id (11-bit or 29-bit)
        unsigned char sizeData;		  	// Data size 0-8
        unsigned char data[8];		    // CAN Data
        unsigned long timestamp;		  // Relative time stamp for package in microseconds
    } canalMsg;
*/


/********************* Qt::DisplayRole  ********************/

    if (role == Qt::DisplayRole)
    {
        QByteArray data_array;
        QString data;

        if(row < m_RxCanFrames->size())
        {
           canframe = m_RxCanFrames->at(row);

           switch(col)
           {
             case 0:
                 return QString("%1").arg(canframe.timestamp,0,10);
             case 1:
                {
                 QString  flag_string;

                 if(canframe.flags & CANAL_IDFLAG_EXTENDED)
                     flag_string = "Ext";
                 else
                     flag_string = "Std";

                 if(canframe.flags & CANAL_IDFLAG_RTR)
                     flag_string.append(":Rtr");

                 if(canframe.flags & CANAL_IDFLAG_STATUS)
                     flag_string = "Status";

                 return flag_string;
                }
             case 2:
                 return QString("0x%1").arg(canframe.id,0,16);
             case 3:
                 return QString("%1").arg(canframe.sizeData,0,10);
             case 4:
                {
                 //data_array = QByteArray::fromRawData((const char*) canframeFD.data, canframeFD.sizeData);
                 data_array = QByteArray::fromRawData((const char*) canframe.data, canframe.sizeData & 0xF);
                 data = data_array.toHex(' ');
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
           if (col != 4) //change text alignment only for cell(1,1)
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

/**************************** Timer *****************************/
void RxFrameTable::timerHit()
{
    //selectR
    //ui->RxMsgTableView->scrollToBottom();
    //ui->RxMsgTableView->selectRow(RxTableModel->m_RxCanFrames.size()-1);
    //ui->RxMsgTableView->setFocus();
}


