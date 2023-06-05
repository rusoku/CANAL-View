/*
 * CANAL view demo software for RUSOKU technologies for TouCAN, TouCAN Marine, TouCAN Duo USB<=>CAN bus converter
 *
 * Copyright (C) 2018 Gediminas Simanskis (gediminas@rusoku.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3.0 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canal.h"
#include "rxworkerthread.h"
#include "QValidator"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QChar>

//void MainWindow::startRxThread()

void MainWindow::exportTableWidgetToCSV()
{
    streamMsg  canframe;
    QString  string;
    QByteArray data_array;
   //QChar ch;

    //QVector<canalMsg>* RxCanFrames = RxTableModel->m_RxCanFrames;
    QVector<streamMsg>* RxCanFrames = m_RxTableModel->getStreamDatabasePointer();

    QString fileName = QFileDialog::getSaveFileName(this,tr("Save file"),QStandardPaths::writableLocation((QStandardPaths::DocumentsLocation)), tr("CSV files (*.csv)"));
    QFile file(fileName);

   // QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    this->setCursor(Qt::WaitCursor);

    //if (f.open(QFile::WriteOnly | QFile::Truncate))
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream <<"TouCAN USB to CAN bus converter raw data log:  " << QDateTime::currentDateTime().toString() << "\n\r";
        stream << "Nr" <<';'<< "Timestamp"<<";"<< "T h:m:sec.ms" <<';'<<"Type (hex)"<<';'<< "Id (hex)"<<';'<<"Length" << ';' << "Data (hex)" << "\n";

    for(qint32 x = 0 ; x < RxCanFrames->size(); x++)
     {
        canframe = RxCanFrames->at(x);
        stream << QString("%1").arg(x+1,0,10) << ';' ;
        stream << QString("%1").arg(canframe.msg.timestamp,0,10) << ';' ;

        stream << QDateTime::fromMSecsSinceEpoch(canframe.stat.time, Qt::UTC).toString("hh:mm:ss.zzz") << ';' ;

        if(canframe.msg.flags & CANAL_IDFLAG_EXTENDED)
            string = "Ext";
        else
            string = "Std";

        if(canframe.msg.flags & CANAL_IDFLAG_RTR)
            string.append(":Rtr");

        if(canframe.msg.flags & CANAL_IDFLAG_STATUS)
            string = "Status";
            string.append(';');

        stream << string;
        stream <<  QString("%1").arg(canframe.msg.id,0,16).toUpper() << ';' << QString("%1").arg(canframe.msg.sizeData,0,10).toUpper() << ';'  ;

        data_array = QByteArray::fromRawData((const char*) canframe.msg.data, canframe.msg.sizeData);
        string = data_array.toHex(' ').toUpper();
        stream << string << "\n";
     }

    }
    file.flush();
    file.close();
    //QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionSave_buffer_triggered()
{
  exportTableWidgetToCSV();
}

