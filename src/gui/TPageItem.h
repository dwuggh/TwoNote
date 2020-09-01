#pragma once

#include "Configs.h"
#include "LineShape.h"
#include "TLineItem.h"
#include "TPage.h"
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QList>
#include <QRectF>
#include <QTransform>

class TPageItem : public QGraphicsItem {
  public:
    TPageItem(int pageNumber = 0, QPointF centralPoint = QPointF(0, 0),
              QGraphicsItem* parent = nullptr);
    int pageNumber;
    QPointF centralPoint;
    QSizeF pageSize;
    QList<LineShape> lines;
    QList<PixmapData> pixmaps;

    // QPointF mapToScene(const QPointF& pagePoint);
    // QPointF mapToPage(const QPointF& scenePoint);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    // void addPixmap(const PixmapData& pixmapData);
    // void addPixmap(QPixmap& pixmap, QPointF position = QPointF(0, 0),
    //            qreal scaleX = 1.0, qreal scaleY = 1.0);
    // const QRectF pageRect();

    // friend QDebug operator<<(QDebug argument, const TPage& obj);
    friend QDataStream& operator>>(QDataStream& in, TPageItem& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPageItem& obj);
    friend QDataStream& operator>>(QDataStream& in, TPageItem*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPageItem* obj);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

  private:
    QList<TLineItem*> lineItems;
    TLineItem* currentLineItem;
    QPainterPath path;
    // QTransform trans;
};
