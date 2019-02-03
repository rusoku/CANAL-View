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
#include "canal_a.h"
#include "rxworkerthread.h"
#include "QValidator"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QFileDialog>

//void MainWindow::startRxThread()

void MainWindow::exportTableWidgetToCSV()
{
    QTableWidget * table = ui->MsgTableWidget;

    //QFile f( "data.csv" );
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save file", QCoreApplication::applicationDirPath(),filters, &defaultFilter);
    QFile f(fileName);


    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &f );
        //QStringList strList;

       QGuiApplication::setOverrideCursor(Qt::WaitCursor);

        // Header
        for( int c = 0; c < table->columnCount(); ++c )
        {
            data << table->horizontalHeaderItem(c)->data(Qt::DisplayRole).toString();
            if (c != (table->columnCount()-1))
                                 data << ",";
        }
        data << "\n" ;

        // Data
        for( int r = 0; r < table->rowCount(); ++r )
        {
            for( int c = 0; c < table->columnCount(); ++c )
            {
              if(table->item( r, c ) != nullptr)
              {
                data << table->item( r, c )->text();
              }
              else
              {
                data << "";
              }

              if (c != (table->columnCount()-1))
                                     data << ",";
            }

          data << "\n" ;
        }
        f.close();

       QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}


void MainWindow::on_actionSave_buffer_triggered()
{
  exportTableWidgetToCSV();
}

