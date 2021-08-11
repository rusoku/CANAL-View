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
#include <QFont>
#include <QSortFilterProxyModel>

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
    ui->le_transmit_delay->setValidator(new QIntValidator(1,999));
    ui->le_txcount->setValidator(new QIntValidator(1,999));
    ui->le_dlc->setValidator(new QIntValidator(0,8));
    //ui->le_burst_cnt->setValidator(new QIntValidator(1,10000));

    //msg ID
    ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHHH");
    //ui->le_msg_ID->setInputMask("BHHHHHHHH");
    ui->cb_msg_ext->setChecked(true);

    // buffer progressBar
    //ui->progressBar->setFocusPolicy(Qt::NoFocus);

//================================= StreamFrameTable =========================

    m_RxTableModel = new RxFrameTable(this);
    ui->RxMsgTableView->setModel(m_RxTableModel);
    //QItemSelectionModel *streamTableSelectionModel = ui->RxMsgTableView->selectionModel();
    streamTableSelectionModel = ui->RxMsgTableView->selectionModel();

    //RxTableModel->blockSignals(true);
    //ui->RxMsgTableView->blockSignals(true);

    ui->RxMsgTableView->setWordWrap(false);
    ui->RxMsgTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    //ui->RxMsgTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);

    /* Selecting only rows */
    ui->RxMsgTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    /* horizontal rows resizing */
    //ui->RxMsgTableView->horizontalHeader()->resizeSections(QHeaderView::Stretch);

    /* Vertical rows resize mode*/
    ui->RxMsgTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->RxMsgTableView->verticalHeader()->setDefaultSectionSize(18);

    /* ??? */
    ui->RxMsgTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->RxMsgTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->RxMsgTableView->setSelectionMode(QAbstractItemView::NoSelection);


    //ui->RxMsgTableView->setStyleSheet("QHeaderView::section {background-color: rgb(51, 51, 255)}, QTableCornerButton::section {background-color: rgb(224, 224, 224}");

/*
    QTableWidget::item{ selection-background-color: red}
    arba:
    QTableWidget::item{ background-color: blue }
    QTableWidget::item:selected{ background-color: red }
*/

    //ui->RxMsgTableView->setStyleSheet("QTableView::item{selection-background-color: red} ");
    ui->RxMsgTableView->setStyleSheet("QTableView::item:selected:active{selection-background-color: grey; selection-color: #FFFFFFFF;  } ");
    ui->RxMsgTableView->setStyleSheet("QTableView::item:selected:inactive{selection-background-color: grey; selection-color: #FFFFFFFF; } ");


    //ui->RxMsgTableView->setStyleSheet("QTableView::item:selected{background: rgb(0 0, 244)}");
    //ui->RxMsgTableView->setStyleSheet("QTableView::item:selected:active{background: rgb(0 0, 244)}");
    //ui->RxMsgTableView->setStyleSheet("QTableView::item:selected:!active{background: rgb(0 0, 244)}");

    //ui->RxMsgTableView->selectRow(0);

    /* get RxCanFramesVector pointer from QTableViewModel */
     m_RxCanFrames = m_RxTableModel->getStreamDatabasePointer();

    //================================= AnalyzerFrameTable =========================

    QSortFilterProxyModel *analyzerFrameProxyModel = new QSortFilterProxyModel;
    AnalyzerTableModel = new AnalyzerFrameTable(this, m_RxCanFrames);

    analyzerFrameProxyModel->setSourceModel(AnalyzerTableModel);

    //ui->AnalyzerTableView->setModel(AnalyzerTableModel);
    ui->AnalyzerTableView->setModel(analyzerFrameProxyModel);

    /* Sorting */
    ui->AnalyzerTableView->setSortingEnabled(true);
    ui->AnalyzerTableView->sortByColumn(3,Qt::AscendingOrder);

    ui->AnalyzerTableView->setWordWrap(false);
    ui->AnalyzerTableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);

    /* Vertical rows resize mode*/
    ui->AnalyzerTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->AnalyzerTableView->verticalHeader()->setDefaultSectionSize(18);

    ui->AnalyzerTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->AnalyzerTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->AnalyzerTableView->setSelectionMode(QAbstractItemView::NoSelection);

    /****** QTableView models signals ******/
    /* Analyze frame after stream table row selection */
    connect(streamTableSelectionModel, &QItemSelectionModel::currentRowChanged, AnalyzerTableModel, &AnalyzerFrameTable::on_doAnalyze);
    /* Scroll down sream table after frame received */
    connect(m_RxTableModel, &RxFrameTable::layoutChanged, this, &MainWindow::StreamTable_scroll_down);

    /* neveikia TODO kodel */
    //connect(m_RxTableModel, &RxFrameTable::newFrameArrived, ui->label_24, &QLabel::setNum);

    /* Rx counter for Label and progressBar */
    connect(m_RxTableModel, SIGNAL(newFrameArrived(int)), ui->label_RxFramesCnt, SLOT(setNum(int)));
    connect(m_RxTableModel, SIGNAL(newFrameArrived(int)), ui->progressBar, SLOT(setValue(int)));

    //================================= TxTableWidget =========================

    //TxTableSelectionModel = ui->TxMsgTableWidget->selectionModel();
    //TxTableSelectionModel->

    //if(TxTableSelectionModel == nullptr)
    //    qDebug() << "TXx table nullptr";

    ui->TxMsgTableWidget->setColumnCount(13);
    ui->TxMsgTableWidget->verticalHeader()->hide();
    ui->TxMsgTableWidget->setHorizontalHeaderLabels(QStringList() << "Id" << "Flag" << "Count" << "Length" << "D0" << "D1" \
                                                << "D2" << "D3" << "D4" << "D5" << "D6" << "D7" << "Transmit delay (ms)");

    QFont font;
    font.setBold(true);
    ui->TxMsgTableWidget->horizontalHeader()->setFont(font);

    ui->TxMsgTableWidget->resizeRowsToContents();
    ui->TxMsgTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->TxMsgTableWidget->resizeColumnToContents(0);
    ui->TxMsgTableWidget->resizeColumnToContents(1);
    //ui->TxMsgTableWidget->resizeColumnToContents(12);

    ui->TxMsgTableWidget->setStyleSheet("QTableView::item:selected:active{selection-background-color: grey; selection-color: #FFFFFFFF;  } ");
    ui->TxMsgTableWidget->setStyleSheet("QTableView::item:selected:inactive{selection-background-color: grey; selection-color: #FFFFFFFF; } ");

/*
    ui->RxMsgTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->RxMsgTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->RxMsgTableView->setSelectionMode(QAbstractItemView::NoSelection);
*/


    ui->TxMsgTableWidget->horizontalHeader()->setStretchLastSection(true);

//    ui->TxMsgTableWidget->setStyleSheet("QHeaderView::section {background-color: rgb(224, 224, 224)},\
//                                       QTableCornerButton::section {background-color: rgb(224, 224, 224}");

    ui->TxMsgTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->TxMsgTableWidget->setFocusPolicy(Qt::NoFocus);
    ui->TxMsgTableWidget->setSelectionMode(QAbstractItemView::QAbstractItemView::SingleSelection);
    ui->TxMsgTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    //msg ID
    ui->le_msg_ID->setText("0x00000000");
    ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHH");
    ui->cb_msg_ext->setChecked(true);

    // Filters
    ui->le_filter_id_11bit->setInputMask("\\0\\x>9HH");
    ui->le_filter_mask_11bit->setInputMask("\\0\\x>9HH");
    ui->le_filter_id_29bit->setInputMask("\\0\\x>BHHHHHHH");
    ui->le_filter_mask_29bit->setInputMask("\\0\\x>BHHHHHHH");

    // 11bit list
    str = ui->le_filter_id_11bit->text();
    str.remove(0,2);
    filter_list_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    // 11bit mask
    str = ui->le_filter_mask_11bit->text();
    str.remove(0,2);
    filter_mask_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    // 29bit list
    str = ui->le_filter_id_29bit->text();
    str.remove(0,2);
    filter_list_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    // 29bit mask
    str = ui->le_filter_mask_29bit->text();
    str.remove(0,2);
    filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    temp_u32 = CanalGetVersion();
    str = QString("%1.%2.%3").arg(temp_u32 & 0xFF).arg((temp_u32>>8) & 0xFF).arg(temp_u32>>16 & 0xFF);
    ui->label_Canal->setText(str);
    temp_u32 = CanalGetDllVersion();
    str = QString("%1.%2.%3").arg(temp_u32 & 0xFF).arg((temp_u32>>8) & 0xFF).arg(temp_u32>>16 & 0xFF);
    ui->label_CanalDll->setText(str);

    // checked
    //ui->cb_AutoBusOff->setChecked(true);
}

void MainWindow::WidgetValuesOpen()
{
    QString str;
    unsigned long temp_u32;

    // enabled
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

    // checked
    //ui->cb_AutoBusOff->setCheckState(Qt::Checked);
    //ui->cb_AutoBusOff->setChecked(true);

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
        str = QString("%1.%2.%3").arg(temp_u32>>24 & 0xFF).arg((temp_u32>>16) & 0xFF).arg(temp_u32>>8 & 0xFF);
        ui->label_Bootloader->setText(str);
    }
    else
        ui->label_Bootloader->setText("-");

    ui->le_OpenParameters->setEnabled(false);
    ui->RxMsgTableView->setFocus();

    TxMsgCnt = 0;
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

    TxMsgCnt = 0;
}

