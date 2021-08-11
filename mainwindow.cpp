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
#include <QList>
#include <QTableView>
#include <QPalette>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString str;

    m_bOpen = false;
    m_bRun = false;

    // Rx CANAL Thread
    countRunning = false;
    infiniteCountRunning = false;
    RxMsgCnt = 0;
    TxMsgCnt = 0;

    m_TxMsgList = new QVector<transmitMsg>;

    // TxMsg data[] init
    for(int x=0; x<8; x++)
         m_TxMsg.msg.data[x] = 0;

    loadSettings();
    WidgetValuesInit();

    // TIMERS
    timerStatus = new QTimer(this);
    connect(timerStatus, &QTimer::timeout,this, &MainWindow::statusTimerTimeout);
    //timerStatistics = new QTimer;
    //connect(timerStatistics, &QTimer::timeout,this, &MainWindow::statisticsTimerTimeout);
    //connect(RxTableModel, &QAbstractTableModel::dataChanged, AnalyzerTableModel, &AnalyzerFrameTable::on_doAnalyze);
    //connect(ui->RxMsgTableView->   
}

MainWindow::~MainWindow()
{
    saveSettings();

    // stop Threads
    stopRxThread();

    // Status timer stop
    timerStatus->stop();
    // Statistics timer stop
    //timerStatistics->stop();

    CanalClose( m_drvHandle );

    m_TxMsgList->clear();
    m_TxMsgList->squeeze();

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

    // Status task let's go
    timerStatus->start(500);
    // Statistics task let's go
    //timerStatistics->start(500);

    m_bOpen = true;
}

//signals
void MainWindow::on_closeButton_clicked()
{
    // Status timer stop
    timerStatus->stop();
     //Statistics timer stop
    //timerStatistics->stop();

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
    m_TxMsg.msg.data[0] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[1] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[2] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[3] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[4] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[5] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[6] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.data[7] = (str.toUInt(&convertOk,16) & 0xFF);
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
    m_TxMsg.msg.sizeData = (str.toUInt(&convertOk,10) & 0xFF);
}

void MainWindow::on_le_txcount_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->le_txcount->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
}


void MainWindow::on_le_txcount_editingFinished()
{
    QString str;
    //bool convertOk;

    ui->le_txcount->setStyleSheet("");
    ui->le_txcount->clearFocus();
    str = ui->le_txcount->text();
    //m_TxMsg.sizeData = (str.toUInt(&convertOk,10) & 0xFF);
}


void MainWindow::on_le_msg_ID_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
//    ui->le_msg_ID->setStyleSheet("QLineEdit {background-color: rgb(255, 240, 179)}");
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
    m_TxMsg.msg.id = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

    if(ui->cb_msg_ext->isChecked())
    {
        m_TxMsg.msg.flags = CANAL_IDFLAG_EXTENDED;
    }
    else
    {
        m_TxMsg.msg.flags = CANAL_IDFLAG_STANDARD;
    }

    if(ui->cb_msg_ext->isChecked())
    {
        m_TxMsg.msg.flags |= CANAL_IDFLAG_RTR;
    }
}

void MainWindow::on_cb_msg_ext_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    if(ui->cb_msg_ext->isChecked())
    {
        ui->le_msg_ID->setMaxLength(10);
        ui->le_msg_ID->setInputMask("\\0\\x>BHHHHHHH");
        ui->le_msg_ID->setText("0x00000000");
        //ui->le_msg_ID->setCursorPosition(0);
    }
    else
    {
        ui->le_msg_ID->setMaxLength(5);
        ui->le_msg_ID->setInputMask("\\0\\x>9HH");
        ui->le_msg_ID->setText("0x000");
        //ui->le_msg_ID->setCursorPosition(0);
    }
   // ui->le_msg_ID->setFocus();
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
    filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);
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
       //QMessageBox::information(this,"Information","Filter 11bit set up successfully");
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
          //QMessageBox::information(this,"Information","Filter 11bit set up successfully");
        }
    }
    else if(ui->rb_filter_11bit_list_mask->isChecked())
    {
        str = ui->le_filter_id_11bit->text();
        str.remove(0,2);
        filter_list_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

        str = ui->le_filter_mask_11bit->text();
        str.remove(0,2);
        filter_mask_11bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

        if(CanalSetFilter11bit(m_drvHandle, FILTER_VALUE, filter_list_11bit, filter_mask_11bit) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
          //QMessageBox::information(this,"Information","Filter 11bit set up successfully");
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
       //QMessageBox::information(this,"Information","Filter 29bit set up successfully");
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
          //QMessageBox::information(this,"Information","Filter 29bit set up successfully");
        }
    }
    else if(ui->rb_filter_29bit_list_mask->isChecked())
    {
        str = ui->le_filter_id_29bit->text();
        str.remove(0,2);
        filter_list_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

        str = ui->le_filter_mask_29bit->text();
        str.remove(0,2);
        filter_mask_29bit = (str.toUInt(&convertOk,16) & 0x1FFFFFFF);

        if(CanalSetFilter29bit(m_drvHandle, FILTER_VALUE, filter_list_29bit, filter_mask_29bit) != CANAL_ERROR_SUCCESS)
        {
          QMessageBox::critical(this,"Error","CANAL communication error");
        }
        else
        {
            qDebug() << QString().asprintf("List = %08X, Mask = %08X",filter_list_29bit, filter_mask_29bit);
          //QMessageBox::information(this,"Information","Filter 29bit set up successfully");
        }
    }
}


void MainWindow::statusTimerTimeout()
{
    canalStatistics  stat;
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

void MainWindow::statisticsTimerTimeout()
{

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
    ui->RxMsgTableView->setFocus();
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
    m_TxMsg.msg.timestamp = (str.toUInt(&convertOk,10));
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

/* Stream database clear */
void MainWindow::on_pb_clear_list_clicked()
{
    //QGuiApplication::setOverrideCursor(Qt::WaitCursor);    
    //this->setCursor(Qt::WaitCursor);

    ui->label_RxFramesCnt->setText(QString::number(0));

    m_RxTableModel->m_StreamCanFrames->clear();
    m_RxTableModel->m_StreamCanFrames->squeeze();
    m_RxTableModel->layoutAboutToBeChanged();
    m_RxTableModel->layoutChanged();

    // analyzer table clear
    on_pushButton_2_clicked();

    //streamTableSelectionModel->clearSelection();
    streamTableSelectionModel->reset();

    //ui->RxMsgTableView->clearSelection();

    //m_RxCanFrames->clear();
    //m_RxCanFrames->squeeze();
    //m_RxTableModel->layoutChanged();

    // clear all buffers
    //RxMsgCnt = 0;

    ui->progressBar->setValue(0);
    emit resetRxThreadCounter();
    //this->setCursor(Qt::ArrowCursor);
    //QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* Analyzer database clear */
void MainWindow::on_pushButton_2_clicked()
{
//    QVector<analyzerMsg>* m_AnalyzerCanFrames = AnalyzerTableModel->getStreamDatabasePointer();

    //QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    //this->setCursor(Qt::WaitCursor);
    /***** Analyzer table ********/
    AnalyzerTableModel->m_AnalyzerCanFrames->clear();
    AnalyzerTableModel->m_AnalyzerCanFrames->squeeze();
    AnalyzerTableModel->layoutAboutToBeChanged();
    AnalyzerTableModel->layoutChanged();


//    m_AnalyzerCanFrames->clear();
//    m_AnalyzerCanFrames->squeeze();
//   AnalyzerTableModel->layoutChanged();

    //this->setCursor(Qt::ArrowCursor);
    //QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
}


void MainWindow::StreamTable_scroll_down()
{
    QModelIndex index;
    index.siblingAtRow(m_RxTableModel->getStreamDatabasePointer()->size()-1);

     //QVector<canalMsg> *temp_RxCanFrames = m_RxTableModel->getStreamDatabasePointer();

    ui->RxMsgTableView->scrollToBottom();

    /* selection */
    //ui->RxMsgTableView->selectRow(m_RxTableModel->getStreamDatabasePointer()->size()-1);
    //streamTableSelectionModel->select(index,QItemSelectionModel::Select);

    //ui->RxMsgTableView->setFocus();
    //ui->RxMsgTableView->selectRow(0);
}
