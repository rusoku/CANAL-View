#include "AnalyzerFrameTable.h"
#include "mainwindow.h"
#include <QChar>
#include <QLatin1Char>
#include <QDateTime>
#include <QTime>
#include <QDebug>
#include <QFont>


AnalyzerFrameTable::AnalyzerFrameTable(QObject *parent, QVector<streamMsg>* streamDatabase)
    : QAbstractTableModel(parent)
{    
    m_StreamCanFrames = streamDatabase;
    m_AnalyzerCanFrames = new QVector<analyzerMsg>;

    //RxFrameTable *table = reinterpreter_cast <RxFrameTable*>(parent);
    //m_StreamCanFrames = table->getStreamDatabasePointer();
    //m_StreamCanFrames = table->m_StreamCanFrames;
}

AnalyzerFrameTable::~AnalyzerFrameTable()
{
    m_AnalyzerCanFrames->clear();
    m_AnalyzerCanFrames->squeeze();
    delete m_AnalyzerCanFrames;
}

/*
QVector<analyzerMsg>* AnalyzerFrameTable::getStreamDatabasePointer(void)
{
    return m_AnalyzerCanFrames;
}
*/

QVariant AnalyzerFrameTable::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        //case 0:
        //   return QString("Timestamp (us)");
        case 0:
           return QString("Rt h:m:sec.ms");
        case 1:
           return QString("Count");
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

    if (role == Qt::FontRole && orientation == Qt::Horizontal)
    {
        QFont font;
        font.setBold(true);
        return font;
    }

    return QVariant();
}

int AnalyzerFrameTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_AnalyzerCanFrames->size();
}

int AnalyzerFrameTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 7;
}

QVariant AnalyzerFrameTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    analyzerMsg  canframe;
    QByteArray data_array;
    QString data;
    QChar ch;

    int row = index.row();
    int col = index.column();

    /********************* Qt::DisplayRole  ********************/

        if (role == Qt::DisplayRole)
        {
            if(row < m_AnalyzerCanFrames->size())
            {
               canframe = m_AnalyzerCanFrames->at(row);

               switch(col)
               {
                //case 0:
                //  return QString("%1").arg( canframe.msg.timestamp,0,10);
                case 0:
                   {
                    return QDateTime::fromMSecsSinceEpoch(canframe.stat.diff_time, Qt::UTC).toString("hh:mm:ss.zzz");
                   }
                case 1:
                  return QString("%1").arg(canframe.stat.count,0,10);
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
                     data_array = QByteArray::fromRawData((const char*) canframe.msg.data, canframe.msg.sizeData).toHex(' ').toUpper();
                     //data = data_array.toHex(' ');
                     //return data.toUpper();
                     return data_array;
                    }
                 case 6:
                  {
                   data_array = QByteArray::fromRawData((const char*) canframe.msg.data, canframe.msg.sizeData);

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

    if( role == Qt::TextAlignmentRole)
    {
        if (col != 5) //change text alignment only for cell(1,1)
        return Qt::AlignHCenter + Qt::AlignVCenter;
    }

    /****************** Qt::SizeHintRole  *****************/

           if( role == Qt::SizeHintRole )
           {
               if (col == 6) //change text alignment only for cell(1,1)
               {
               }
           }

    return QVariant();
}

/***************************** SLOTS ****************************************/

void AnalyzerFrameTable::on_doAnalyze(const QModelIndex &current,const QModelIndex &previous)
{
    //Q_UNUSED(current);
    Q_UNUSED(previous)
    //qDebug() << QString::number(current.row());

    analyzerMsg tmp_analyzerMsg = {};
    streamMsg tmp_streamMsg = {};
    qint32 streamSelectionIndex = current.row();
/*
    qDebug() << "";
    qDebug() << "******************************************************************";
    qDebug() << QString("streamSelectionIndex %1").arg(streamSelectionIndex,0,10);
    qDebug() << QString("streamDataSize %1").arg(m_StreamCanFrames->size(),0,10);
    qDebug() << QString("analyzerDataSize %1").arg(m_AnalyzerCanFrames->size(),0,10);
    qDebug() << "******************************************************************";
    qDebug() << "";
*/

    m_AnalyzerCanFrames->clear();
    m_AnalyzerCanFrames->squeeze();

    for(qint32 i = 0; i< streamSelectionIndex+1; ++i) // curent selected row in streamAnalyzer
    {
        //qDebug() << QString("stream counter = %1").arg(i,0,10);

        tmp_streamMsg = m_StreamCanFrames->at(i);

        if(lookForFrame(&tmp_streamMsg))
        {
         //qDebug() << "Radom.Pakeiciam";
        }
        else
        {
         //qDebug() << "Neradom.Irasom nauja";
         tmp_analyzerMsg.msg = tmp_streamMsg.msg;
         tmp_analyzerMsg.stat.time = tmp_streamMsg.stat.time;
         tmp_analyzerMsg.stat.diff_time = 0;
         tmp_analyzerMsg.stat.count = 1;
         m_AnalyzerCanFrames->append(tmp_analyzerMsg);
        }
    }

    layoutAboutToBeChanged();
    layoutChanged();

    /*
    for (qint32 i = 0; i < m_AnalyzerCanFrames->size(); ++i)
    {
     temp_rxmsg = m_AnalyzerCanFrames->at(i);

        if((temp_rxmsg.msg.id == rxmsg.id) && (temp_rxmsg.msg.flags == rxmsg.flags))
          {
            temp_rxmsg.msg = rxmsg;
            temp_rxmsg.stat.count++;

            temp_rxmsg.stat.diff_time = QDateTime::currentMSecsSinceEpoch() - temp_rxmsg.stat.time;
            temp_rxmsg.stat.time = QDateTime::currentMSecsSinceEpoch();

            m_AnalyzerCanFrames->replace(i,temp_rxmsg);
            goto end;
          }
    }
    */

    //temp_rxmsg.msg = rxmsg;
    //temp_rxmsg.stat.time = QDateTime::currentMSecsSinceEpoch();
    //temp_rxmsg.stat.diff_time = 0;
    //m_AnalyzerCanFrames->append(temp_rxmsg);
//end:

}

bool AnalyzerFrameTable::lookForFrame(streamMsg *streamMesage)
{
    analyzerMsg tmp_analyzerMsg = {};    

    for(qint32 j = 0; j< m_AnalyzerCanFrames->size(); j++)
    {
      tmp_analyzerMsg = m_AnalyzerCanFrames->at(j);

      //qDebug() << "";
      //qDebug() << QString("Stream Id %1").arg(streamFrame->id,0,16);
      //qDebug() << QString("Analyzer Id %1").arg(tmp_analyzerMsg.msg.id,0,16);

      if((tmp_analyzerMsg.msg.id  ==  streamMesage->msg.id ) && (tmp_analyzerMsg.msg.flags  == streamMesage->msg.flags))
      {          
       tmp_analyzerMsg.msg = streamMesage->msg;
       tmp_analyzerMsg.stat.count++;
       tmp_analyzerMsg.stat.diff_time = streamMesage->stat.time - tmp_analyzerMsg.stat.time;
       tmp_analyzerMsg.stat.time = streamMesage->stat.time;
       m_AnalyzerCanFrames->replace(j,tmp_analyzerMsg);
       //qDebug() << "Pass";
       //qDebug() << "";
       return true;
      }
    }

    //qDebug() << QString("Stream Id %1").arg(streamFrame->id,0,16);
    //qDebug() << QString("Analyzer Id %1").arg(tmp_analyzerMsg.msg.id,0,16);
    //qDebug() << "Unpass";
    //qDebug() << "";
    return false;
}

