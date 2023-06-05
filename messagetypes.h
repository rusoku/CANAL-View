#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H

#include <QtGlobal>
#include "canal.h"

enum
{
    insert = false,
    append = true
};


/* analyzer message */
typedef struct _analyzerStat {
    qint32  count;
    qint64  time;
    qint64  diff_time;
} analyzerStat;

typedef struct _analyzerMsg{
    canalMsg     msg;
    analyzerStat stat;
} analyzerMsg;

/* stream message */
typedef struct _streamStat{
    qint64  time;
}streamStat;

typedef struct _streamMsg{
    canalMsg   msg;
    streamStat stat;
}streamMsg;

/* transmit message */

typedef struct _transmitStat{
    qint16  count;
}transmitStat;

typedef struct _transmitMsg{
    canalMsg msg;
    transmitStat  stat;
}transmitMsg;

#endif // MESSAGETYPES_H
