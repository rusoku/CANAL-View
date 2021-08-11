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


void MainWindow::SendWidgetToTxMsg(bool insert_type)
{
//Q_UNUSED(TransmitMessageFrame);

    QString str;
    bool convertOk;
    transmitMsg  canMsg;
    QTableWidgetItem *tbl;

    qint32 row;

    if(insert_type == append){
     row = ui->TxMsgTableWidget->rowCount();
    }
    else{
     row = ui->TxMsgTableWidget->currentRow();
     if(row < 0) return;
    }

    ui->TxMsgTableWidget->insertRow(row);

    // message ID
    str = ui->le_msg_ID->text();
    str.remove(0,2);
    canMsg.msg.id = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    // TxTable "ID"
    str = (ui->le_msg_ID->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 0, tbl);

    // message STD:EXT:RTR
    if(ui->cb_msg_ext->isChecked())
    {
        canMsg.msg.flags = CANAL_IDFLAG_EXTENDED;
    }
    else
    {
        canMsg.msg.flags = CANAL_IDFLAG_STANDARD;
    }

    if(ui->cb_msg_rtr->isChecked())
    {
        canMsg.msg.flags |= CANAL_IDFLAG_RTR;
    }

    // TxTable "STD:EXT:RTR"
    if(canMsg.msg.flags & CANAL_IDFLAG_EXTENDED)
        str = "Ext";
    else
        str = "Sds";

    if(canMsg.msg.flags & CANAL_IDFLAG_RTR)
        str.append(":Rtr");

    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 1, tbl);

    // count
    str = ui->le_txcount->text();
    canMsg.stat.count = (str.toUInt(&convertOk,10));

    // TxTable "Count"
    str = (ui->le_txcount->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 2, tbl);

    // message DLC
    str = ui->le_dlc->text();
    canMsg.msg.sizeData = (str.toUInt(&convertOk,10) & 0xFF);

    // TxTable "dlc"
    str = (ui->le_dlc->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 3, tbl);

/*************** DATA **********************/

    // message DATA
    str = ui->le_data0->text();
    str.remove(0,2);
    canMsg.msg.data[0] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data0"
    str = (ui->le_data0->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 4, tbl);

    str = ui->le_data1->text();
    str.remove(0,2);
    canMsg.msg.data[1] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data1"
    str = (ui->le_data1->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 5, tbl);

    str = ui->le_data2->text();
    str.remove(0,2);
    canMsg.msg.data[2] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data2"
    str = (ui->le_data2->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 6, tbl);

    str = ui->le_data3->text();
    str.remove(0,2);
    canMsg.msg.data[3] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data3"
    str = (ui->le_data3->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 7, tbl);

    str = ui->le_data4->text();
    str.remove(0,2);
    canMsg.msg.data[4] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data4"
    str = (ui->le_data4->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 8, tbl);

    str = ui->le_data5->text();
    str.remove(0,2);
    canMsg.msg.data[5] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data5"
    str = (ui->le_data5->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 9, tbl);

    str = ui->le_data6->text();
    str.remove(0,2);
    canMsg.msg.data[6] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data6"
    str = (ui->le_data6->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 10, tbl);

    str = ui->le_data7->text();
    str.remove(0,2);
    canMsg.msg.data[7] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data7"
    str = (ui->le_data7->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 11, tbl);

    /**************************************************************/

    // transmit delay
    str = ui->le_transmit_delay->text();
    canMsg.msg.timestamp = (str.toUInt(&convertOk,10));

    // TxTable "Transmit delay"
    str = (ui->le_transmit_delay->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(row, 12, tbl);


    if (insert_type == append)
    {
     m_TxMsgList->append(canMsg);
     ui->TxMsgTableWidget->selectRow(row);
    }
    else
    {
     m_TxMsgList->insert(row,canMsg);
    }

}

// delete last item
void MainWindow::on_pb_DeleteLast_clicked()
{
    ui->TxMsgTableWidget->setRowCount(0);

    m_TxMsgList->clear();
    m_TxMsgList->squeeze();

    emit resetTxThreadCounter();
}

/* add Tx row */
void MainWindow::on_pb_AddItem_clicked()
{
    SendWidgetToTxMsg(append);
}

/* insert TX row */
void MainWindow::on_pushButton_4_clicked()
{
    SendWidgetToTxMsg(insert);
}


/* delete Tx row */
void MainWindow::on_pushButton_3_clicked()
{
     qint32 row;

     row = ui->TxMsgTableWidget->currentRow();

     if( row >= 0)
     {
      ui->TxMsgTableWidget->removeRow(row);
      m_TxMsgList->remove(row);
     }
}

void MainWindow::updateInfiniteTxCount(unsigned long cnt)
{
    Q_UNUSED(cnt);
    ui->label_TxFramesCnt->setText(QString::number(++TxMsgCnt));
}

void MainWindow::on_pb_msg_send_clicked()
{
    QString str;

    if(!m_bOpen)
    {
       QMessageBox::critical(this,"Error","CANAL is closed");
       return;
    }

//    qDebug() << m_TxMsg.id << m_TxMsg.sizeData << m_TxMsg.data[0] << m_TxMsg.data[1] << m_TxMsg.data[2] << m_TxMsg.data[3] \
//                                               << m_TxMsg.data[4] << m_TxMsg.data[5] << m_TxMsg.data[6] << m_TxMsg.data[7] \
//                                               << m_TxMsg.timestamp;

    startTxThread();
}



