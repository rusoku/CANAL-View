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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QList>
#include <QThread>
#include <QTableWidgetItem>
#include "canal.h"
#include "rxworkerthread.h"
#include "txworkerthread.h"
#include "dialogabout.h"
#include "dialoginitstring.h"

Q_DECLARE_METATYPE(structCanalMsg)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // application open.close state
    bool m_bOpen;
    // run threads
    bool m_bRun;

    // CANAL messages
    //QList<canalMsg> m_RxMsgList;
    //QList<canalMsg> m_TxMsgList;

    QList<canalMsg> *m_RxMsgList;
    QList<canalMsg> *m_TxMsgList;

    canalMsg m_TxMsg;
    canalMsg m_RxMsg;
    unsigned int m_burst_cnt;

    //settings
    void saveSettings();
    void loadSettings();

signals:
    void stopRxThreadSignal();
    void resetRxThreadCounter();

    void stopTxThreadSignal();
    void resetTxThreadCounter();

private slots:

    void on_closeButton_clicked();
    void on_openButton_clicked();
    void on_le_data0_textChanged(const QString &arg1);
    void on_le_data0_editingFinished();
    void on_le_data1_textChanged(const QString &arg1);
    void on_le_data1_editingFinished();
    void on_le_data2_textChanged(const QString &arg1);
    void on_le_data2_editingFinished();
    void on_le_data3_textChanged(const QString &arg1);
    void on_le_data3_editingFinished();
    void on_le_data4_textChanged(const QString &arg1);
    void on_le_data4_editingFinished();
    void on_le_data5_textChanged(const QString &arg1);
    void on_le_data5_editingFinished();
    void on_le_data6_textChanged(const QString &arg1);
    void on_le_data6_editingFinished();
    void on_le_data7_textChanged(const QString &arg1);
    void on_le_data7_editingFinished();
    void on_le_dlc_editingFinished();
    void on_le_dlc_textChanged(const QString &arg1);
    //void on_le_burst_cnt_editingFinished();
    //void on_le_burst_cnt_textChanged(const QString &arg1);
    void on_le_msg_ID_textChanged(const QString &arg1);
    void on_le_msg_ID_editingFinished();
    void on_cb_msg_ext_stateChanged(int arg1);
    void on_le_filter_id_11bit_textChanged(const QString &arg1);
    void on_le_filter_id_11bit_editingFinished();
    void on_le_filter_mask_11bit_textChanged(const QString &arg1);
    void on_le_filter_mask_11bit_editingFinished();
    void on_le_filter_id_29bit_textChanged(const QString &arg1);
    void on_le_filter_id_29bit_editingFinished();
    void on_le_filter_mask_29bit_textChanged(const QString &arg1);
    void on_le_filter_mask_29bit_editingFinished();
    void on_pb_filter_set_11bit_clicked();
    void on_pb_filter_set_29bit_10_clicked();

    // staus & statistics Timer
    void statusTimerTimeout();
    void statisticsTimerTimeout();

    // Rx CANAL Thread
    void updateInfiniteCount(unsigned long cnt, canalMsg msg);
    // Tx message sent
    void updateInfiniteTxCount(unsigned long cnt);
    //void infiniteCountFinished();
    void startRxThread();
    void stopRxThread(); //GS

    void startTxThread();
    void stopTxThread(); //GS

    // Tx CANAL Thread


    void on_pb_clear_list_clicked();

    // TEST button
    //void on_pushButton_clicked(/*canalMsg msg*/);
    void on_actionSave_buffer_triggered();
    void on_actionClose_triggered();
    void on_pb_msg_send_clicked();

    void on_startButton_clicked();
    void on_stopButton_clicked();

    void on_pb_DeleteLast_clicked();
    void on_pb_AddItem_clicked();

    void on_le_transmit_delay_editingFinished();

    void on_le_transmit_delay_textChanged(const QString &arg1);

    void on_actionAbout_triggered();

    void on_actionInit_string_triggered();

private:
    Ui::MainWindow *ui;

    void exportTableWidgetToCSV();

    //Widgets values settings
    void WidgetValuesInit();
    void WidgetValuesOpen();
    void WidgetValuesClose();

    // Canal DLL device handle
    int m_drvHandle;

    // Widgets value settings
    QSettings *settings;

    //Filter
    unsigned long   filter_list_11bit;
    unsigned long   filter_mask_11bit;
    unsigned long   filter_list_29bit;
    unsigned long   filter_mask_29bit;

    QTimer  *timerStatistics;
    QTimer  *timerStatus;

    // Rx CANAL Thread
    bool countRunning;
    bool infiniteCountRunning;
    unsigned long RxMsgCnt;
    unsigned long TxMsgCnt;

    // Rx thread
    QThread *m_RxWorkerThread;
    RxWorkerThreadInfinite *m_RxWorker ;

    // Tx worker
    QThread *m_TxWorkerThread;
    TxWorkerThreadInfinite *m_TxWorker ;

    // MsgTableWidget
    QTableWidgetItem *tbl;
    int m_rowCountCurrent = 0;

    //MsgSend
    void SendWidgetToTxMsg(canalMsg &TransmitMessageFrame);

    DialogAbout *dlgAbout;
    DialogInitString *dlgInitString;
};


#endif // MAINWINDOW_H
