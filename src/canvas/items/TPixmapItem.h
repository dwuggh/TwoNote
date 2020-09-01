#pragma once

#include <QDataStream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QPointF>

class TPixmapItem : public QGraphicsPixmapItem {
  public:
    explicit TPixmapItem(const QPixmap& pixmap = QPixmap(),
                         QPointF position = QPointF(0, 0), qreal scaleX = 1.0,
                         qreal scaleY = 1.0, QGraphicsItem* parent = nullptr);

    qreal scaleX;
    qreal scaleY;
    void setScale(qreal sx, qreal sy);
    friend QDataStream& operator>>(QDataStream& in, TPixmapItem& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPixmapItem& obj);
    friend QDataStream& operator>>(QDataStream& in, TPixmapItem*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPixmapItem* obj);
};
