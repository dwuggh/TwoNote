#ifndef TWONOTE_TPAGE_H
#define TWONOTE_TPAGE_H

#include <QDebug>
#include <QDataStream>
#include <QPainterPath>
#include <QImage>
#include <QPixmap>
#include "Configs.h"
#include "LineShape.h"
#include <QSizeF>
#include <QRectF>
#include <QGraphicsTextItem>
// #include "TTextItem.h"

class PixmapData {
public:
    PixmapData(QPixmap pixmap = QPixmap(), QPointF position = QPointF(0, 0), qreal scaleX = 1.0, qreal scaleY = 1.0);
    QPointF position;
    qreal scaleX;
    qreal scaleY;
    QPixmap pixmap;
    friend QDebug operator<<(QDebug argument, const PixmapData &obj);
    friend QDataStream &operator>>(QDataStream &in, PixmapData &obj);
    friend QDataStream &operator<<(QDataStream &out, const PixmapData &obj);
};

class TPage {


public:
    TPage(int pageNumber = 0, QSizeF pageSize = config.pageView.pageSize);

    // start from 0
    int pageNumber;
    QSizeF pageSize;
    QList<LineShape> lines;
    QList<PixmapData> pixmaps;
    // QList<QGraphicsTextItem> texts;

    // like QGraphicsView::mapToScene
    QPointF mapToScene(const QPointF &pagePoint);
    QPointF mapToPage(const QPointF &scenePoint);
    void addLine(const LineShape& sceneLine);
    void addPixmap(const PixmapData& pixmapData);
    const QRectF pageRect();
    LineShapes sceneLineShapes();

    friend QDebug operator<<(QDebug argument, const TPage &obj);
    friend QDataStream &operator>>(QDataStream &in, TPage &obj);
    friend QDataStream &operator<<(QDataStream &out, const TPage &obj);
private:
    /* QList<QPixmap> images; */
    /* QList<QTextItem> texts; */

};


#endif
