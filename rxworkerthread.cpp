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


//=========================== Infinite Thread ===========================
RxWorkerThreadInfinite::RxWorkerThreadInfinite(int handler) : m_running(true)
{
    m_drvHandle = handler;
    m_msg = new canalMsg;
    cnt = 0;
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
    while(m_running)
    {
        //PortableSleep::msleep(1);
        //QThread::msleep(1);

        QCoreApplication::processEvents();

        if(CanalBlockingReceive(m_drvHandle, m_msg , 100) == CANAL_ERROR_SUCCESS)
        {            
            if(m_running)
            {               
             if(++cnt < 10000000) emit updateInfiniteCount(cnt, *m_msg);
            }
        }
    }
    emit finished();
}

//========================= Thread driving =========================
void MainWindow::startRxThread()
{
    m_RxWorkerThread = new QThread;
    m_RxWorker       = new RxWorkerThreadInfinite(m_drvHandle);
    m_RxWorker->moveToThread(m_RxWorkerThread);

    connect(m_RxWorkerThread, &QThread::started, m_RxWorker, &RxWorkerThreadInfinite::doWork);
    connect(m_RxWorker, &RxWorkerThreadInfinite::finished, m_RxWorkerThread, &QThread::quit);
    connect(m_RxWorker, &RxWorkerThreadInfinite::finished, m_RxWorker, &RxWorkerThreadInfinite::deleteLater);
    connect(m_RxWorkerThread, &QThread::finished, m_RxWorkerThread, &QThread::deleteLater);

    //connect(m_RxWorker, &RxWorkerThreadInfinite::updateInfiniteCount, this, &MainWindow::updateInfiniteCountStream); //Qt::BlockingQueuedConnection);
    connect(m_RxWorker, &RxWorkerThreadInfinite::updateInfiniteCount, m_RxTableModel, &RxFrameTable::updateFrame);

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



