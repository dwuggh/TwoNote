#pragma once

#include "Configs.h"
#include "State.h"
#include "TLineItem.h"
#include "TPixmapItem.h"
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QList>
#include <QMimeData>
#include <QRectF>
#include <QTextDocument>
#include <QTransform>
#include <QUrl>

class TPage : public QGraphicsItem {
  public:
    TPage(int pageNumber = 0, QPointF centralPoint = QPointF(0, 0),
          QGraphicsItem* parent = nullptr);
    int pageNumber;
    QPointF centralPoint;
    QSizeF pageSize;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    // void addPixmap(const PixmapData& pixmapData);
    // void addPixmap(QPixmap& pixmap, QPointF position = QPointF(0, 0),
    //            qreal scaleX = 1.0, qreal scaleY = 1.0);

    // friend QDebug operator<<(QDebug argument, const TPage& obj);
    friend QDataStream& operator>>(QDataStream& in, TPage& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPage& obj);
    friend QDataStream& operator>>(QDataStream& in, TPage*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPage* obj);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent*) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent*) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent*) override;
    void dropEvent(QGraphicsSceneDragDropEvent*) override;

  private:
    QList<TLineItem*> lineItems;
    QList<TPixmapItem*> pixmapItems;
    TLineItem* currentLineItem;
    QPointF currentPoint;
    // QTransform trans;
};
