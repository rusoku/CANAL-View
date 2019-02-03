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



void MainWindow::SendWidgetToTxMsg(canalMsg &TransmitMessageFrame)
{
Q_UNUSED(TransmitMessageFrame);

    QString str;
    bool convertOk;
    canalMsg  canMsg;
    QTableWidgetItem *tbl;

    int rowCount = ui->TxMsgTableWidget->rowCount();
    ui->TxMsgTableWidget->insertRow(rowCount);

    // TxTable "Nr"
    str.sprintf("%d",rowCount);
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 0, tbl);

    // TxTable "Dir"
    str = "T";
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 1, tbl);

    // message ID
    str = ui->le_msg_ID->text();
    str.remove(0,2);
    canMsg.id = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    // TxTable "ID"
    str = (ui->le_msg_ID->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 2, tbl);

    // message STD:EXT:RTR
    if(ui->cb_msg_ext->isChecked())
    {
        canMsg.flags = CANAL_IDFLAG_EXTENDED;
    }
    else
    {
        canMsg.flags = CANAL_IDFLAG_STANDARD;
    }

    if(ui->cb_msg_rtr->isChecked())
    {
        canMsg.flags |= CANAL_IDFLAG_RTR;
    }

    // TxTable "STD:EXT:RTR"
    if(canMsg.flags & CANAL_IDFLAG_EXTENDED)
        str = "EXT";
    else
        str = "STD";

    if(canMsg.flags & CANAL_IDFLAG_RTR)
        str.append(":RTR");

    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 3, tbl);


    // message DLC
    str = ui->le_dlc->text();
    canMsg.sizeData = (str.toUInt(&convertOk,10) & 0xFF);

    // TxTable "dlc"
    str = (ui->le_dlc->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 4, tbl);

    // message DATA
    str = ui->le_data0->text();
    str.remove(0,2);
    canMsg.data[0] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data0"
    str = (ui->le_data0->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 5, tbl);

    str = ui->le_data1->text();
    str.remove(0,2);
    canMsg.data[1] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data1"
    str = (ui->le_data1->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 6, tbl);

    str = ui->le_data2->text();
    str.remove(0,2);
    canMsg.data[2] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data2"
    str = (ui->le_data2->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 7, tbl);

    str = ui->le_data3->text();
    str.remove(0,2);
    canMsg.data[3] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data3"
    str = (ui->le_data3->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 8, tbl);

    str = ui->le_data0->text();
    str.remove(0,4);
    canMsg.data[4] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data4"
    str = (ui->le_data4->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 9, tbl);

    str = ui->le_data5->text();
    str.remove(0,2);
    canMsg.data[5] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data5"
    str = (ui->le_data5->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 10, tbl);

    str = ui->le_data6->text();
    str.remove(0,2);
    canMsg.data[6] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data6"
    str = (ui->le_data6->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 11, tbl);

    str = ui->le_data7->text();
    str.remove(0,2);
    canMsg.data[7] = (str.toUInt(&convertOk,16) & 0xFF);

    // TxTable "data7"
    str = (ui->le_data7->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 12, tbl);

    // transmit delay
    str = ui->le_transmit_delay->text();
    canMsg.timestamp = (str.toUInt(&convertOk,10));

    // TxTable "Transmit delay"
    str = (ui->le_transmit_delay->text());
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->TxMsgTableWidget->setItem(rowCount, 13, tbl);

    TransmitMessageFrame = canMsg;
}

// delete last item
void MainWindow::on_pb_DeleteLast_clicked()
{
    int rowCount = ui->TxMsgTableWidget->rowCount();

    if(rowCount)
    {
        ui->TxMsgTableWidget->removeRow(rowCount-1);
    }

    if(!m_TxMsgList->isEmpty())
    {
        m_TxMsgList->removeLast();
    }
}

// add item
void MainWindow::on_pb_AddItem_clicked()
{
    SendWidgetToTxMsg(m_TxMsg);
    m_TxMsgList->append(m_TxMsg);
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



