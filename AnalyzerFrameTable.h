#ifndef ANALYZERFRAMETABLE_H
#define ANALYZERFRAMETABLE_H

#include <QAbstractTableModel>
#include <QTimer>
#include <QVector>
#include "canal.h"
#include "canal_a.h"
#include "messagetypes.h"


class AnalyzerFrameTable : public QAbstractTableModel
{
    Q_OBJECT

private:
    bool lookForFrame(streamMsg *streamFrame);

public:
    explicit AnalyzerFrameTable(QObject *parent = nullptr,  QVector<streamMsg>* streamDatabase = nullptr);
    ~AnalyzerFrameTable();

//    QVector<analyzerMsg>* getStreamDatabasePointer(void);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVector<analyzerMsg> *m_AnalyzerCanFrames;
    QVector<streamMsg> *m_StreamCanFrames;

//public slots:

public  slots:
    void on_doAnalyze(const QModelIndex &current, const QModelIndex &previous);
};

#endif // ANALYZERFRAMETABLE_H
