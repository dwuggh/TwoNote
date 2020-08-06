//
// Created by dwuggh on 8/4/20.
//

#ifndef TWONOTE_LINESHAPE_H
#define TWONOTE_LINESHAPE_H

#include <QList>
#include <QPointF>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <utility>
#include <QDebug>
#include <QDataStream>
#include <qimage.h>

class LineShape {
public:
    QList<QPointF> points;
    QColor color;
    qreal width = 1.0;
    explicit LineShape(qreal width = 1.0, QColor color = QColor(Qt::black));

    bool operator ==(const LineShape &other) const;
    void quadPaintLine(QPainter &painter);
    void append(const QPointF &point);

    friend QDebug operator<<(QDebug argument, const LineShape &obj);
    friend QDataStream &operator>>(QDataStream &in, LineShape &obj);
    friend QDataStream &operator<<(QDataStream &out, const LineShape &obj);
private:
    QPen pen;
    void setPen();
};

typedef QList<LineShape> LineShapes;

#endif //TWONOTE_LINESHAPE_H
