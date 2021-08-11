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

#include "txworkerthread.h"
#include "portablesleep.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

//=========================== Infinite Thread ===========================
TxWorkerThreadInfinite::TxWorkerThreadInfinite(int handler, QVector<transmitMsg> *canalMSGlist) : m_running(true)
{
    m_drvHandle = handler;
    m_msg = new transmitMsg;
    m_TxcanalMSGlist = canalMSGlist;
    m_MsgCnt = 0;
}


void TxWorkerThreadInfinite::doWork()
{
    canalMsg TxMsg;
    qint32 count = 0;
    //qint16 delay;

        cnt = m_TxcanalMSGlist->count();

        for(qint32 x=0; x < cnt; x++)
        {
          TxMsg = m_TxcanalMSGlist->at(x).msg;
          count =  m_TxcanalMSGlist->at(x).stat.count;

          if(count > 999)
                  count = 999;

            for(qint64 z=0; z<count; z++)
            {
              CanalSend(m_drvHandle, &TxMsg);
              emit updateInfiniteTxCount(++m_MsgCnt);
              QCoreApplication::processEvents();
            }

          //QCoreApplication::processEvents();
          //qApp->processEvents();
        }
    emit finished();
}


//========================= Thread driving =========================
void MainWindow::startTxThread()
{
    m_TxWorkerThread = new QThread;
    m_TxWorker       = new TxWorkerThreadInfinite(m_drvHandle, m_TxMsgList);
    m_TxWorker->moveToThread(m_TxWorkerThread);

    /*pradeda darba*/
    connect(m_TxWorkerThread, &QThread::started, m_TxWorker, &TxWorkerThreadInfinite::doWork);
    /* susinaikina abu */
    connect(m_TxWorker, &TxWorkerThreadInfinite::finished, m_TxWorkerThread, &QThread::quit);
    connect(m_TxWorker, &TxWorkerThreadInfinite::finished, m_TxWorker, &TxWorkerThreadInfinite::deleteLater);
    connect(m_TxWorkerThread, &QThread::finished, m_TxWorkerThread, &QThread::deleteLater);

    /* po kiekvieno issiusto Msg signalina atvaizdavimui */
    connect(m_TxWorker, &TxWorkerThreadInfinite::updateInfiniteTxCount, this, &MainWindow::updateInfiniteTxCount); // canal msg is sent
    m_TxWorkerThread->start();
}



