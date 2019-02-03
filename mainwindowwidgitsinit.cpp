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


void MainWindow::WidgetValuesInit()
{
    QString str;
    bool convertOk;
    unsigned long temp_u32;

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->closeButton->setEnabled(false);
    ui->label_Hardware->setText("-");
    ui->label_Firmware->setText("-");
    ui->label_Canal->setText("-");
    ui->label_CanalDll->setText("-");
    ui->label_Vendor->setText("-");
    ui->label_Bootloader->setText("-");
    ui->rb_errorFree->setChecked(true);
    ui->statusBar->showMessage("CANAL status: Closed");

    // set RxMsg Table index focus
    ui->tabWidget->setCurrentIndex(0);

    ui->le_data0->setInputMask("\\0\\x>HH");
    ui->le_data1->setInputMask("\\0\\x>HH");
    ui->le_data2->setInputMask("\\0\\x>HH");
    ui->le_data3->setInputMask("\\0\\x>HH");
    ui->le_data4->setInputMask("\\0\\x>HH");
    ui->le_data5->setInputMask("\\0\\x>HH");
    ui->le_data6->setInputMask("\\0\\x>HH");
    ui->le_data7->setInputMask("\\0\\x>HH");
    ui->le_dlc->setValidator(new QIntValidator(0,8));
    //ui->le_burst_cnt->setValidator(new QIntValidator(1,10000));

    //msg ID
    ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHHH");
    //ui->le_msg_ID->setInputMask("BHHHHHHHH");
    ui->cb_msg_ext->setChecked(true);


    //================================= RxTableWidget =========================
    ui->MsgTableWidget->setColumnCount(14);
    //ui->MsgTableWidget->setRowCount(20);
    ui->MsgTableWidget->verticalHeader()->hide();
    ui->MsgTableWidget->setHorizontalHeaderLabels(QStringList()<< "   Nr   " << " Dir " << "Id" << "Flag" << "Dlc" << "D0" << "D1" \
                                                               << "D2" << "D3" << "D4" << "D5" << "D6" << "D7" << "Timestamp (uS)");
    ui->MsgTableWidget->resizeRowsToContents();
    ui->MsgTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->MsgTableWidget->resizeColumnsToContents();
    ui->MsgTableWidget->resizeColumnToContents(0);
    ui->MsgTableWidget->resizeColumnToContents(1);
    ui->MsgTableWidget->resizeColumnToContents(4);

    ui->MsgTableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->MsgTableWidget->setStyleSheet("QHeaderView::section {background-color: rgb(224, 224, 224)},\
                                       QTableCornerButton::section {background-color: rgb(224, 224, 224}");

    ui->MsgTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->MsgTableWidget->setFocusPolicy(Qt::NoFocus);
    ui->MsgTableWidget->setSelectionMode(QAbstractItemView::NoSelection);


    //================================= TxTableWidget =========================

    ui->TxMsgTableWidget->setColumnCount(14);
    ui->TxMsgTableWidget->verticalHeader()->hide();
    ui->TxMsgTableWidget->setHorizontalHeaderLabels(QStringList()<< " Nr " << " Dir " << "Id" << "Flag" << "Dlc" << "D0" << "D1" \
                                                               << "D2" << "D3" << "D4" << "D5" << "D6" << "D7" << "Transmit delay (mS)");

    ui->TxMsgTableWidget->resizeRowsToContents();
    ui->TxMsgTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TxMsgTableWidget->resizeColumnToContents(0);
    ui->TxMsgTableWidget->resizeColumnToContents(1);
    ui->TxMsgTableWidget->resizeColumnToContents(4);

    ui->TxMsgTableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->TxMsgTableWidget->setStyleSheet("QHeaderView::section {background-color: rgb(224, 224, 224)},\
                                       QTableCornerButton::section {background-color: rgb(224, 224, 224}");

    ui->TxMsgTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TxMsgTableWidget->setFocusPolicy(Qt::NoFocus);
    ui->TxMsgTableWidget->setSelectionMode(QAbstractItemView::NoSelection);


    //msg ID
    ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHHH");
    //ui->le_msg_ID->setInputMask("BHHHHHHHH");
    ui->cb_msg_ext->setChecked(true);

    // Filters
    //ui->le_data0->setInputMask("\\0\\x>HH");
    ui->le_filter_id_11bit->setInputMask("\\0\\x>9HH");
    ui->le_filter_mask_11bit->setInputMask("\\0\\x>9HH");
    ui->le_filter_id_29bit->setInputMask("\\0\\x>BHHHHHHHH");
    ui->le_filter_mask_29bit->setInputMask("\\0\\x>BHHHHHHHH");

    // 11bit list
    str = ui->le_filter_id_11bit->text();
    str.remove(0,2);
    filter_list_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    // 11bit mask
    str = ui->le_filter_mask_11bit->text();
    str.remove(0,2);
    filter_mask_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    // 29bit list
    str = ui->le_filter_id_29bit->text();
    str.remove(0,2);
    filter_list_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    // 29bit mask
    str = ui->le_filter_mask_29bit->text();
    str.remove(0,2);
    filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    temp_u32 = CanalGetVersion();
    str.sprintf("%d.%d.%d",temp_u32 & 0xFF, (temp_u32>>8) & 0xFF, (temp_u32>>16) & 0xFF);
    ui->label_Canal->setText(str);
    temp_u32 = CanalGetDllVersion();
    str.sprintf("%d.%d.%d",temp_u32 & 0xFF, (temp_u32>>8) & 0xFF, (temp_u32>>16) & 0xFF);
    ui->label_CanalDll->setText(str);
}

void MainWindow::WidgetValuesOpen()
{
    QString str;
    unsigned long temp_u32;

    ui->cb_SilentMode->setEnabled(false);
    ui->cb_LoopbackMode->setEnabled(false);
    ui->cb_DisRetransmition->setEnabled(false);
    ui->cb_WakeUpMode->setEnabled(false);
    ui->cb_AutoBusOff->setEnabled(false);
    ui->cb_TimMode->setEnabled(false);
    ui->cb_RxFifoLocked->setEnabled(false);
    ui->cb_TxFifoPriority->setEnabled(false);
    ui->cb_EnStatusMessage->setEnabled(false);
    ui->cb_TimestampDelay->setEnabled(false);

    ui->openButton->setEnabled(false);
    ui->closeButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->statusBar->showMessage("CANAL status: Open");
    ui->label_RxFramesCnt->setText(QString::number(0));
    ui->label_TxFramesCnt->setText(QString::number(0));

    str = QString::fromStdString(CanalGetVendorString());
    QStringList parts = str.split(';');
    ui->label_Hardware->setText(parts.at(0));
    ui->label_Firmware->setText(parts.at(1));
    ui->label_Vendor->setText(parts.at(2));

    if(CanalGetBootloaderVersion(m_drvHandle, &temp_u32) == CANAL_ERROR_SUCCESS)
    {
        str.sprintf("%d.%d.%d",(temp_u32>>24) & 0xFF, (temp_u32>>16) & 0xFF, (temp_u32>>8) & 0xFF);
        ui->label_Bootloader->setText(str);
    }
    else
        ui->label_Bootloader->setText("-");

    ui->le_OpenParameters->setEnabled(false);
}

void MainWindow::WidgetValuesClose()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->openButton->setEnabled(true);
    ui->closeButton->setEnabled(false);
    ui->statusBar->showMessage("CANAL status: Closed");

    ui->label_Hardware->setText("-");
    ui->label_Firmware->setText("-");
 //   ui->label_Canal->setText("-");
 //   ui->label_CanalDll->setText("-");
    ui->label_Vendor->setText("-");
    ui->label_Bootloader->setText("-");

    ui->cb_SilentMode->setEnabled(true);
    ui->cb_LoopbackMode->setEnabled(true);
    ui->cb_DisRetransmition->setEnabled(true);
    ui->cb_WakeUpMode->setEnabled(true);
    ui->cb_AutoBusOff->setEnabled(true);
    ui->cb_TimMode->setEnabled(true);
    ui->cb_RxFifoLocked->setEnabled(true);
    ui->cb_TxFifoPriority->setEnabled(true);
    ui->cb_EnStatusMessage->setEnabled(true);
    ui->cb_TimestampDelay->setEnabled(true);

    ui->lb_ReceivedFrames->setText("0");
    ui->lb_ReceivedData->setText("0");
    ui->lb_TransmitedFrames->setText("0");
    ui->lb_transmitedData->setText("0");
    ui->lb_Overruns->setText("0");
    ui->lb_Warnings->setText("0");
    ui->lb_BussOff->setText("0");

    ui->rb_filter_11bit_accept_all->setChecked(true);
    ui->rb_filter_29bit_accept_all->setChecked(true);

    ui->label_RxFramesCnt->setText(QString::number(0));
    ui->label_TxFramesCnt->setText(QString::number(0));

    ui->le_OpenParameters->setEnabled(true);
}

