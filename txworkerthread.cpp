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
TxWorkerThreadInfinite::TxWorkerThreadInfinite(int handler, QList<canalMsg> *canalMSGlist) : m_running(true)
{
    m_drvHandle = handler;
    m_msg = new canalMsg;
    cnt = 0;
    m_TxcanalMSGlist = canalMSGlist;
    m_MsgCnt = 0;
}

void TxWorkerThreadInfinite::stopWork()
{
    m_running = false;
}

void TxWorkerThreadInfinite::doWork()
{
    canalMsg TxMsg;
    int x, cnt;
   //unsigned long MsgCnt = 0;

   // qDebug() << "TxThread started!";

        cnt = m_TxcanalMSGlist->count();

        for(x=0; x < cnt; x++)
        {
          TxMsg = m_TxcanalMSGlist->at(x);
          //qDebug() << "TxcanalMSGlist = " << TxMsg.id;
          CanalSend(m_drvHandle, &TxMsg);
          emit updateInfiniteTxCount(++m_MsgCnt);
          qApp->processEvents();
        }

        //while(!m_TxcanalMSGlist->isEmpty())
       // {
              //TxMsg = m_TxcanalMSGlist->takeFirst();
         //       TxMsg = m_TxcanalMSGlist-
        //      qDebug() << "TxcanalMSGlist = " << TxMsg.id;
              //emit frameSent();// GS frame is sent
       //       qApp->processEvents();
       // }

    //qDebug() << "TxThread finished!";
    emit finished();
}


//========================= Thread driving =========================
void MainWindow::startTxThread()
{
    m_TxWorkerThread = new QThread;
    m_TxWorker       = new TxWorkerThreadInfinite(m_drvHandle, m_TxMsgList);
    m_TxWorker->moveToThread(m_TxWorkerThread);

    connect(m_TxWorkerThread, &QThread::started, m_TxWorker, &TxWorkerThreadInfinite::doWork);
    connect(m_TxWorker, &TxWorkerThreadInfinite::finished, m_TxWorkerThread, &QThread::quit);
    connect(m_TxWorker, &TxWorkerThreadInfinite::finished,  m_TxWorker, &TxWorkerThreadInfinite::deleteLater);
    connect(m_TxWorkerThread, &QThread::finished, m_TxWorkerThread, &QThread::deleteLater);
    connect(m_TxWorker, &TxWorkerThreadInfinite::updateInfiniteTxCount, this, &MainWindow::updateInfiniteTxCount); // canal msg is sent
    connect(this, &MainWindow::stopTxThreadSignal, m_TxWorker, &TxWorkerThreadInfinite::stopWork);//GS
    connect(this, &MainWindow::resetTxThreadCounter, m_TxWorker, &TxWorkerThreadInfinite::resetTxCounter);//GS // reset TX counter

    m_TxWorkerThread->start();
    infiniteCountRunning = true;
}

void MainWindow::stopTxThread()
{
    emit  stopTxThreadSignal();
    infiniteCountRunning = false;
}

void TxWorkerThreadInfinite::resetTxCounter()
{
    m_MsgCnt = 0;
}

/*
void TxWorkerThreadInfinite::doWork()
{
    int cnt;
    canalMsg TxMsg;

    qDebug() << "TxThread started!";

    while (m_running)
    {
        qApp->processEvents();
        PortableSleep::msleep(500);

        if(!m_TxcanalMSGlist->isEmpty())
        {
            cnt = m_TxcanalMSGlist->count();
            for(int x= 0; x < cnt; x++)
            {
              TxMsg = m_TxcanalMSGlist->takeFirst();
              qDebug() << "TxcanalMSGlist = " << TxMsg.id;
              //emit frameSent();// GS kai freimas issiustas
              qApp->processEvents();
            }
        }
        else
        {
            //qDebug() << "TxcanalMSGlist tuscias";
            continue;
        }
    }

    qDebug() << "TxThread finished!";
    emit finished();
}
*/
