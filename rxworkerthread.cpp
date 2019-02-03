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

#include "rxworkerthread.h"
#include "portablesleep.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

/*
RxWorkerThread::RxWorkerThread(int start, int end)
{
    m_countStart = start;
    m_countEnd   = end;
}

void RxWorkerThread::doWork()
{
    for (int i = m_countStart; i <= m_countEnd; i++) {
        emit updateCount(i);
        qApp->processEvents();
        PortableSleep::msleep(1000);
    }
    emit finished();    
}
*/

//=========================== Infinite Thread ===========================
RxWorkerThreadInfinite::RxWorkerThreadInfinite(int handler, QList<canalMsg> *canalMSGlist) : m_running(true)
{
    m_drvHandle = handler;
    m_msg = new canalMsg;
    cnt = 0;
    m_RxcanalMSGlist = canalMSGlist;
}

void RxWorkerThreadInfinite::stopWork()
{
    m_running = false;
}

void RxWorkerThreadInfinite::resetRxCounter()
{
    cnt = 0;
}

void RxWorkerThreadInfinite::doWork()
{
    while (m_running)
    {
        qApp->processEvents();

        if(CanalBlockingReceive(m_drvHandle, m_msg , 500) == CANAL_ERROR_SUCCESS)
        {
            if(m_running)
            {
              emit updateInfiniteCount(++cnt, *m_msg);
            }
        }
    }

    //qDebug() << "RxThread finished!";
    emit finished();
}


//========================= Thread driving =========================
void MainWindow::startRxThread()
{
    m_RxWorkerThread = new QThread;
    m_RxWorker       = new RxWorkerThreadInfinite(m_drvHandle, m_RxMsgList);
    m_RxWorker->moveToThread(m_RxWorkerThread);

    connect(m_RxWorkerThread, &QThread::started, m_RxWorker, &RxWorkerThreadInfinite::doWork);
    connect(m_RxWorker, &RxWorkerThreadInfinite::finished, m_RxWorkerThread, &QThread::quit);
    connect(m_RxWorker, &RxWorkerThreadInfinite::finished,  m_RxWorker, &RxWorkerThreadInfinite::deleteLater);
    connect(m_RxWorkerThread, &QThread::finished, m_RxWorkerThread, &QThread::deleteLater);
    connect(m_RxWorker, &RxWorkerThreadInfinite::updateInfiniteCount, this, &MainWindow::updateInfiniteCount);
    connect(this, &MainWindow::stopRxThreadSignal, m_RxWorker, &RxWorkerThreadInfinite::stopWork);//GS
    connect(this, &MainWindow::resetRxThreadCounter, m_RxWorker, &RxWorkerThreadInfinite::resetRxCounter);//GS

    m_RxWorkerThread->start();
    infiniteCountRunning = true;
}

void MainWindow::stopRxThread()
{
    emit  stopRxThreadSignal();
    infiniteCountRunning = false;
    //m_workerThread->wait(150);
}



