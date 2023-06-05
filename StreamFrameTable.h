#ifndef RXFRAMETABLE_H
#define RXFRAMETABLE_H

#include <QAbstractTableModel>
#include <QTimer>
#include <QVector>
#include "canal.h"
#include "messagetypes.h"


class RxFrameTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RxFrameTable(QObject *parent);
    ~RxFrameTable();

    QVector<streamMsg>* getStreamDatabasePointer(void);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &parent) const override;    

    QVector<streamMsg>* m_StreamCanFrames;

signals:
    void newFrameArrived(int cnt);

private:    

    bool    updateFrameTimerEnable = false;
    QTimer  *m_timer;
    quint32  m_FramesCnt;

public slots:
    void updateFrame(quint32 cnt, canalMsg rxmsg);

private slots:
    void updateFrameTimerCallback(void);
    void timerHit();

};

#endif // RXFRAMETABLE_H
