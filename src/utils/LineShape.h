#pragma once

#include <QColor>
#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <qimage.h>
#include <utility>

class LineShape {
  public:
    LineShape(qreal width = 3.0, QColor color = QColor(Qt::black));
    LineShape(const LineShape& other);

    QList<QPointF> points;
    QColor color;
    qreal width = 1.0;
    QPen pen;
    bool operator==(const LineShape& other) const;
    void append(const QPointF& point);

    friend QDebug operator<<(QDebug argument, const LineShape& obj);
    friend QDataStream& operator>>(QDataStream& in, LineShape& obj);
    friend QDataStream& operator<<(QDataStream& out, const LineShape& obj);

  private:
    void setPen();
};

typedef QList<LineShape> LineShapes;
