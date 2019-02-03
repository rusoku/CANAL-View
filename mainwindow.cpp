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
#include "dialogabout.h"
#include "canal.h"
#include "canal_a.h"
#include "rxworkerthread.h"
#include "QValidator"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString str;

    m_bOpen = false;
    m_bRun = false;

    // Rx CANAL Thread
    countRunning = false;
    infiniteCountRunning = false;
    RxMsgCnt = 0;
    TxMsgCnt = 0;

    // Rx&Tx CANAL buffers
    //QList<canalMsg> *m_TxMsgList;
    m_RxMsgList = new QList<canalMsg>;
    m_TxMsgList = new QList<canalMsg>;

    // TxMsg data[] init
    for(int x=0; x<8; x++)
         m_TxMsg.data[x] = 0;

    ui->setupUi(this);

    loadSettings();
    WidgetValuesInit();

    // TIMERS
    timerStatus = new QTimer(this);
    connect(timerStatus, &QTimer::timeout,this, &MainWindow::statusTimerTimeout);
    timerStatistics = new QTimer;
    connect(timerStatistics, &QTimer::timeout,this, &MainWindow::statisticsTimerTimeout);
}

MainWindow::~MainWindow()
{
    saveSettings();

    // stop Threads
    stopRxThread();

    // Status timer stop
    timerStatus->stop();
    // Statistics timer stop
    timerStatistics->stop();

    CanalClose( m_drvHandle );

    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    unsigned long openflags = 0;
    QString str;

    openflags = ((ui->cb_SilentMode->isChecked())           ? 1 : 0 ) |
                ((ui->cb_LoopbackMode->isChecked())         ? 2 : 0 ) |
                ((ui->cb_DisRetransmition->isChecked())     ? 4 : 0 ) |
                ((ui->cb_WakeUpMode->isChecked())           ? 8 : 0 ) |
                ((ui->cb_AutoBusOff->isChecked())           ? 16: 0 ) |
                ((ui->cb_TimMode->isChecked())              ? 32: 0 ) |
                ((ui->cb_RxFifoLocked->isChecked())         ? 64: 0 ) |
                ((ui->cb_TxFifoPriority->isChecked())       ? 128: 0) |
                ((ui->cb_EnStatusMessage->isChecked())      ? 256: 0) |
                ((ui->cb_TimestampDelay->isChecked())       ? 512: 0);

    str = ui->le_OpenParameters->text();
    //cstr = str.toStdString().c_str();

    if(( m_drvHandle = CanalOpen(str.toStdString().c_str(),openflags)) <= 0)
    {
        QMessageBox::critical(this,"Error","failed to open USB-CAN interface");
        m_bOpen = false;
        return;
    }

    WidgetValuesOpen();

    // start Threads
    startRxThread();
//    startTxThread();

    // Status task let's go
    timerStatus->start(250);
    // Statistics task let's go
    timerStatistics->start(250);

    m_bOpen = true;
}

//signals
void MainWindow::on_closeButton_clicked()
{
    // Status timer stop
    timerStatus->stop();
     //Statistics timer stop
    timerStatistics->stop();

    // stop Threads
    stopRxThread();
//    stopTxThread();

    CanalClose( m_drvHandle );
    m_bOpen = false;
    WidgetValuesClose();
}

// led_data0 ======================
void MainWindow::on_le_data0_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    //ui->le_data0->setCursorPosition(2);
    ui->le_data0->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data0_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data0->setStyleSheet("");
    ui->le_data0->clearFocus();
    str = ui->le_data0->text();
    str.remove(0,2);
    m_TxMsg.data[0] = (str.toUInt(&convertOk,16) & 0xFF);
}

void MainWindow::on_le_data1_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data1->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data1_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data1->setStyleSheet("");
    ui->le_data1->clearFocus();
    str = ui->le_data1->text();
    str.remove(0,2);
    m_TxMsg.data[1] = (str.toUInt(&convertOk,16) & 0xFF);
}

void MainWindow::on_le_data2_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data2->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data2_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data2->setStyleSheet("");
    ui->le_data2->clearFocus();
    str = ui->le_data2->text();
    str.remove(0,2);
    m_TxMsg.data[2] = (str.toUInt(&convertOk,16) & 0xFF);
}


void MainWindow::on_le_data3_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data3->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data3_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data3->setStyleSheet("");
    ui->le_data3->clearFocus();
    str = ui->le_data3->text();
    str.remove(0,2);
    m_TxMsg.data[3] = (str.toUInt(&convertOk,16) & 0xFF);
}

void MainWindow::on_le_data4_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data4->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data4_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data4->setStyleSheet("");
    ui->le_data4->clearFocus();
    str = ui->le_data4->text();
    str.remove(0,2);
    m_TxMsg.data[4] = (str.toUInt(&convertOk,16) & 0xFF);
}


void MainWindow::on_le_data5_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data5->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data5_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data5->setStyleSheet("");
    ui->le_data5->clearFocus();
    str = ui->le_data5->text();
    str.remove(0,2);
    m_TxMsg.data[5] = (str.toUInt(&convertOk,16) & 0xFF);
}


void MainWindow::on_le_data6_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data6->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data6_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data6->setStyleSheet("");
    ui->le_data6->clearFocus();
    str = ui->le_data6->text();
    str.remove(0,2);
    m_TxMsg.data[6] = (str.toUInt(&convertOk,16) & 0xFF);
}



void MainWindow::on_le_data7_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_data7->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_data7_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_data7->setStyleSheet("");
    ui->le_data7->clearFocus();
    str = ui->le_data7->text();
    str.remove(0,2);
    m_TxMsg.data[7] = (str.toUInt(&convertOk,16) & 0xFF);
}

void MainWindow::on_le_dlc_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_dlc->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}


void MainWindow::on_le_dlc_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_dlc->setStyleSheet("");
    ui->le_dlc->clearFocus();
    str = ui->le_dlc->text();
    m_TxMsg.sizeData = (str.toUInt(&convertOk,10) & 0xFF);
}

/*
void MainWindow::on_le_burst_cnt_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_burst_cnt->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}
*/

/*
void MainWindow::on_le_burst_cnt_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_burst_cnt->setStyleSheet("");
    ui->le_burst_cnt->clearFocus();
    str = ui->le_burst_cnt->text();
    m_burst_cnt = (str.toUInt(&convertOk,10) & 0xFFFF);
}
*/

void MainWindow::on_le_msg_ID_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_msg_ID->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_msg_ID_editingFinished()
{
    QString str;
    bool convertOk;
    //unsigned long tmp_long;

    ui->le_msg_ID->setStyleSheet("");
    ui->le_msg_ID->clearFocus();
    str = ui->le_msg_ID->text();

    str.remove(0,2);
    m_TxMsg.id = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

    if(ui->cb_msg_ext->isChecked())
    {
        m_TxMsg.flags = CANAL_IDFLAG_EXTENDED;
    }
    else
    {
        m_TxMsg.flags = CANAL_IDFLAG_STANDARD;
    }

    if(ui->cb_msg_ext->isChecked())
    {
        m_TxMsg.flags |= CANAL_IDFLAG_RTR;
    }
}

void MainWindow::on_cb_msg_ext_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    if(ui->cb_msg_ext->isChecked())
    {
        ui->le_msg_ID->setMaxLength(11);
        ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHHH");
        ui->le_msg_ID->setText("0x000000000");
        //ui->le_msg_ID->setCursorPosition(0);
    }
    else
    {
        ui->le_msg_ID->setMaxLength(5);
        ui->le_msg_ID->setInputMask("\\0\\x>9HH");
        ui->le_msg_ID->setText("0x000");
        //ui->le_msg_ID->setCursorPosition(0);
    }
    ui->le_msg_ID->setFocus();
}

void MainWindow::on_le_filter_id_11bit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_filter_id_11bit->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_filter_id_11bit_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_filter_id_11bit->setStyleSheet("");
    ui->le_filter_id_11bit->clearFocus();
    str = ui->le_filter_id_11bit->text();
    str.remove(0,2);
    filter_list_11bit = (str.toUInt(&convertOk,16) & 0x7FF);
}

void MainWindow::on_le_filter_mask_11bit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_filter_mask_11bit->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_filter_mask_11bit_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_filter_mask_11bit->setStyleSheet("");
    ui->le_filter_mask_11bit->clearFocus();
    str = ui->le_filter_mask_11bit->text();
    str.remove(0,2);
    filter_mask_11bit = (str.toUInt(&convertOk,16) & 0x7FF);
}

void MainWindow::on_le_filter_id_29bit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_filter_id_29bit->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_filter_id_29bit_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_filter_id_29bit->setStyleSheet("");
    ui->le_filter_id_29bit->clearFocus();
    str = ui->le_filter_id_29bit->text();
    str.remove(0,2);
    filter_list_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);
}

void MainWindow::on_le_filter_mask_29bit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_filter_mask_29bit->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}

void MainWindow::on_le_filter_mask_29bit_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_filter_mask_29bit->setStyleSheet("");
    ui->le_filter_mask_29bit->clearFocus();
    str = ui->le_filter_mask_29bit->text();
    str.remove(0,2);
    filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);
}

void MainWindow::on_pb_filter_set_11bit_clicked()
{
    QString str;
    bool convertOk;

    if(!m_bOpen)
    {
       QMessageBox::critical(this,"Error","CANAL is closed");
       return;
    }

    if(ui->rb_filter_11bit_reject_all->isChecked())
    {
     if(CanalSetFilter11bit(m_drvHandle, FILTER_REJECT_ALL, 0, 0) != CANAL_ERROR_SUCCESS)
     {
       QMessageBox::critical(this,"Error","CANAL communication error");
     }
     else
     {
       QMessageBox::information(this,"Information","Filter 11bit set up successfully");
     }
    }
    else if (ui->rb_filter_11bit_accept_all->isChecked())
    {
        if(CanalSetFilter11bit(m_drvHandle, FILTER_ACCEPT_ALL, 0, 0) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
          QMessageBox::information(this,"Information","Filter 11bit set up successfully");
        }
    }
    else if(ui->rb_filter_11bit_list_mask->isChecked())
    {
        str = ui->le_filter_id_11bit->text();
        str.remove(0,2);
        filter_list_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

        str = ui->le_filter_mask_11bit->text();
        str.remove(0,2);
        filter_mask_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

        if(CanalSetFilter11bit(m_drvHandle, FILTER_VALUE, filter_list_11bit, filter_mask_11bit) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
          QMessageBox::information(this,"Information","Filter 11bit set up successfully");
        }
    }
}


void MainWindow::on_pb_filter_set_29bit_10_clicked()
{
    QString str;
    bool convertOk;

    if(!m_bOpen)
    {
       QMessageBox::critical(this,"Error","CANAL is closed");
       return;
    }

    if(ui->rb_filter_29bit_reject_all->isChecked())
    {
     if(CanalSetFilter29bit(m_drvHandle, FILTER_REJECT_ALL, 0, 0) != CANAL_ERROR_SUCCESS)
     {
       QMessageBox::critical(this,"Error","CANAL communication error");
     }
     else
     {
       QMessageBox::information(this,"Information","Filter 29bit set up successfully");
     }
    }
    else if (ui->rb_filter_29bit_accept_all->isChecked())
    {
        if(CanalSetFilter29bit(m_drvHandle, FILTER_ACCEPT_ALL, 0, 0) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
          QMessageBox::information(this,"Information","Filter 29bit set up successfully");
        }
    }
    else if(ui->rb_filter_29bit_list_mask->isChecked())
    {
        str = ui->le_filter_id_29bit->text();
        str.remove(0,2);
        filter_list_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

        str = ui->le_filter_mask_29bit->text();
        str.remove(0,2);
        filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFFF);

        if(CanalSetFilter29bit(m_drvHandle, FILTER_VALUE, filter_list_29bit, filter_mask_29bit) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
          QMessageBox::information(this,"Information","Filter 29bit set up successfully");
        }
    }
}

void MainWindow::on_pb_clear_list_clicked()
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    ui->label_RxFramesCnt->setText(QString::number(0));
    ui->label_TxFramesCnt->setText(QString::number(0));

    // Table reset
    ui->MsgTableWidget->clearSelection();
    ui->MsgTableWidget->disconnect();
    ui->MsgTableWidget->clearContents();
    ui->MsgTableWidget->setRowCount(0); //
    ui->MsgTableWidget->reset();
    m_rowCountCurrent = 0;

    // Table reset
    ui->TxMsgTableWidget->clearSelection();
    ui->TxMsgTableWidget->disconnect();
    ui->TxMsgTableWidget->clearContents();
    ui->TxMsgTableWidget->setRowCount(0); //
    ui->TxMsgTableWidget->reset();

    // clear all buffers
    RxMsgCnt = 0;
    TxMsgCnt = 0;
    m_RxMsgList->clear();
    m_TxMsgList->clear();

    emit resetRxThreadCounter();
    emit resetTxThreadCounter();

    QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::statusTimerTimeout()
{
    canalStatus	 status;

    if ( CanalGetStatus( m_drvHandle,&status ) == CANAL_ERROR_SUCCESS )
    {
     if( status.channel_status & CANAL_STATUS_BUS_OFF )
         ui->rb_busOff->setChecked(true);
     else if( status.channel_status & CANAL_STATUS_PASSIVE )
         ui->rb_passive->setChecked(true);
     else if( status.channel_status & CANAL_STATUS_BUS_WARN )
         ui->rb_warning->setChecked(true);
     else
         ui->rb_errorFree->setChecked(true);

      ui->label_RxErr->setText(QString::number((status.channel_status>>8)&0xFF));
      ui->label_TxErr->setText(QString::number(status.channel_status & 0xFF));
    }
}

void MainWindow::statisticsTimerTimeout()
{
    canalStatistics  stat;

   if ( CanalGetStatistics( m_drvHandle,&stat ) == CANAL_ERROR_SUCCESS )
   {
        ui->lb_ReceivedFrames->setText(QString::number(stat.cntReceiveFrames));
        ui->lb_ReceivedData->setText(QString::number(stat.cntReceiveData));
        ui->lb_TransmitedFrames->setText(QString::number(stat.cntTransmitFrames));
        ui->lb_transmitedData->setText(QString::number(stat.cntTransmitData));
        ui->lb_Overruns->setText(QString::number(stat.cntOverruns));
        ui->lb_Warnings->setText(QString::number(stat.cntBusWarnings));
        ui->lb_BussOff->setText(QString::number(stat.cntBusOff));
   }
}

// Rx CANAL Thread
//void MainWindow::infiniteCountFinished()
//{
//    infiniteCountRunning = false;
//}

void MainWindow::updateInfiniteCount(unsigned long cnt, canalMsg rxmsg)
{
    //qDebug() << QString("ID=%1,Timestamp=%2,Cnt=%3").arg(rxmsg.id).arg(rxmsg.timestamp).arg(rxmsg.sizeData);
    ui->label_RxFramesCnt->setText(QString::number(cnt));//GS

//    canalMsg rxmsg;
    QString str;
    QTableWidgetItem *tbl;
    int rowCount = m_rowCountCurrent++;
    RxMsgCnt++;

    rowCount = ui->MsgTableWidget->rowCount();
    ui->MsgTableWidget->insertRow(rowCount);

    //ui->MsgTableWidget->scrollToBottom();

    str.sprintf("%d",RxMsgCnt);
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 0, tbl);

    tbl = new QTableWidgetItem(QString("R"));
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 1 , tbl);

    str.sprintf("0x%08X",rxmsg.id);
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 2, tbl);


    if(rxmsg.flags & CANAL_IDFLAG_EXTENDED)
        str = "EXT";
    else
        str = "STD";

    if(rxmsg.flags & CANAL_IDFLAG_RTR)
        str.append(":RTR");

    if(rxmsg.flags & CANAL_IDFLAG_STATUS)
        str = ":STATUS";

    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 3, tbl);

    str.sprintf("%d",rxmsg.sizeData);
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 4, tbl);

    for(int x=0; x< rxmsg.sizeData; x++)
    {
      str.sprintf("0x%02X",rxmsg.data[x]);
      tbl = new QTableWidgetItem(str);
      tbl->setTextAlignment(Qt::AlignCenter);
      ui->MsgTableWidget->setItem(rowCount, x+5, tbl);
    }

    for(int x = rxmsg.sizeData; x< 8 ; x++)
    {
      str = "-";
      tbl = new QTableWidgetItem(str);
      tbl->setTextAlignment(Qt::AlignCenter);
      ui->MsgTableWidget->setItem(rowCount, x+5, tbl);
    }

    str.sprintf("0x%08X",rxmsg.timestamp);
    tbl = new QTableWidgetItem(str);
    tbl->setTextAlignment(Qt::AlignCenter);
    ui->MsgTableWidget->setItem(rowCount, 13, tbl);

    //tbl->setTextAlignment(QItemSelectionModel::SelectCurrent);
    //ui->MsgTableWidget->scrollToBottom();
}


void MainWindow::on_actionClose_triggered()
{
    close();
}


void MainWindow::on_startButton_clicked()
{
    if(!m_bOpen)
    {
       QMessageBox::critical(this,"Error","CANAL is closed");
       return;
    }

    if(CanalInterfaceStart(m_drvHandle) == CANAL_ERROR_SUCCESS)
    {
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
}

void MainWindow::on_stopButton_clicked()
{
    if(!m_bOpen)
    {
       QMessageBox::critical(this,"Error","CANAL is closed");
       return;
    }

    if(CanalInterfaceStop(m_drvHandle) == CANAL_ERROR_SUCCESS)
    {
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
    }
}


void MainWindow::on_le_transmit_delay_editingFinished()
{
    QString str;
    bool convertOk;

    ui->le_transmit_delay->setStyleSheet("");
    ui->le_transmit_delay->clearFocus();
    str = ui->le_transmit_delay->text();
    m_TxMsg.timestamp = (str.toUInt(&convertOk,10));
}

void MainWindow::on_le_transmit_delay_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_transmit_delay->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}



void MainWindow::on_actionAbout_triggered()
{
    dlgAbout = new DialogAbout;
    dlgAbout->setWindowTitle("About");
    dlgAbout->setWindowFlags(Qt::WindowTitleHint);
    dlgAbout->exec();
    delete dlgAbout;
}

void MainWindow::on_actionInit_string_triggered()
{
    dlgInitString = new DialogInitString;
    dlgInitString->setWindowTitle("Init string");
    dlgInitString->setWindowFlags(Qt::WindowTitleHint);
    dlgInitString->exec();
    delete dlgInitString;
}

