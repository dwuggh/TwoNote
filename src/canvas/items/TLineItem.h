#pragma once

#include <QDebug>
#include <QGraphicsPathItem>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QTransform>

class TLineItem : public QGraphicsPathItem {
  public:
    explicit TLineItem(const QPointF& point = QPointF(0, 0), QGraphicsItem* parent = nullptr);
    void addPoint(const QPointF& point, bool endOfLine = false);
    void refresh();
    void setPen(qreal width = 3.0, const QColor& color = QColor(Qt::black));
    static inline qreal squareDist(const QPointF& p1, const QPointF& p2);
    friend QDebug operator<<(QDebug argument, const TLineItem& obj);
    friend QDataStream& operator>>(QDataStream& in, TLineItem& obj);
    friend QDataStream& operator<<(QDataStream& out, const TLineItem& obj);
    friend QDataStream& operator>>(QDataStream& in, TLineItem*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TLineItem* obj);

  private:
    QPainterPath path;
    QList<QPointF> points;
    unsigned int recentPCounter;
    QPointF currentPoint;
    QPointF lastPoint;
};
