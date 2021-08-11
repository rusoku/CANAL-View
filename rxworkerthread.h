/*
 * CANAL view demo software for RUSOKU technologies TouCAN USB<=>CAN bus converter
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


#ifndef RXWORKERTHREAD_H
#define RXWORKERTHREAD_H

#include "canal.h"
#include "canal_a.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>


//======================= RxWorkerThreadInfinite ===================
class RxWorkerThreadInfinite : public QObject
{
    Q_OBJECT

public:
    explicit RxWorkerThreadInfinite(int handler);

public slots:
    void doWork();
    void stopWork();
    void resetRxCounter();

signals:
    void updateInfiniteCount(quint32, canalMsg);
    void finished();

private:
    canalMsg *m_msg;
    int  m_drvHandle;
    bool m_running;   
    quint32 cnt;
};

#endif // RXWORKERTHREAD_H
