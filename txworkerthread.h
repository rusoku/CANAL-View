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

#ifndef TXWORKERTHREAD_H
#define TXWORKERTHREAD_H

#include "canal.h"
#include "canal_a.h"
#include "portablesleep.h"
#include <QMainWindow>
#include <QObject>
#include <QThread>


//======================= RxWorkerThreadInfinite ===================
class   TxWorkerThreadInfinite : public QObject
{
    Q_OBJECT

public:
    explicit TxWorkerThreadInfinite(int handler, QList<canalMsg> *canalMSGlist);

public slots:
    void doWork();
    void stopWork();
    void resetTxCounter();

signals:
    void updateInfiniteTxCount(unsigned long);
    void finished();
    void frameSent();

private:
    canalMsg *m_msg;
    int  m_drvHandle;
    bool m_running;
    unsigned long cnt, m_MsgCnt;
    QList<canalMsg> *m_TxcanalMSGlist;
};

#endif // TXWORKERTHREAD_H
