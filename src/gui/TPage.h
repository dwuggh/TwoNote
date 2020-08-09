#ifndef TWONOTE_TPAGE_H
#define TWONOTE_TPAGE_H

#include <QDebug>
#include <QDataStream>
#include <QPainterPath>
#include <QImage>
#include <QPixmap>
#include "LineShape.h"
#include <QSizeF>

class TPage {


public:
    TPage(int pageNumber = 0, QSizeF pageSize = QSizeF(1920, 3000));

    // start from 0
    int pageNumber;
    QSizeF pageSize;
    QList<LineShape> lines;

    QPointF pagePosToScenePos(QPointF& pagePoint);
    QPointF scenePosToPagePos(QPointF& scenePoint);
    void addLine(const LineShape& sceneLine);
    LineShapes sceneLineShapes();

    friend QDebug operator<<(QDebug argument, const TPage &obj);
    friend QDataStream &operator>>(QDataStream &in, TPage &obj);
    friend QDataStream &operator<<(QDataStream &out, const TPage &obj);
private:
    /* QList<QPixmap> images; */
    /* QList<QTextItem> texts; */

};


#endif
