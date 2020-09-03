#pragma once

#include "State.h"
#include <QCursor>
#include <QDataStream>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMap>
#include <QPointF>

enum HandleType {
    handleTopLeft,
    handleTopMiddle,
    handleTopRight,
    handleMiddleLeft,
    handleMiddleRight,
    handleBottomLeft,
    handleBottomMiddle,
    handleBottomRight,
    noHandle,
};

class TPixmapItem : public QGraphicsPixmapItem {
  public:
    explicit TPixmapItem(const QPixmap& pixmap = QPixmap(), QPointF position = QPointF(0, 0), qreal scaleX = 1.0,
                         qreal scaleY = 1.0, QGraphicsItem* parent = nullptr);

    void setScale(qreal sx, qreal sy, bool composite = false);
    friend QDataStream& operator>>(QDataStream& in, TPixmapItem& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPixmapItem& obj);
    friend QDataStream& operator>>(QDataStream& in, TPixmapItem*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPixmapItem* obj);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent*) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent*) override;

  private:
    QMap<HandleType, QRectF> handles;
    HandleType selectedHandle;
    void setupHandles();
    HandleType selectHandle(const QPointF& p);
    void interactiveResize(const QPointF& pos);
};
