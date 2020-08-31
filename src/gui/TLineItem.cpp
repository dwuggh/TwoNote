#include "TLineItem.h"

TLineItem::TLineItem(const QPointF& point, QGraphicsItem* parent)
    : QGraphicsPathItem(parent) {
    currentPoint = point;
    path = QPainterPath();
    path.moveTo(point);
    this->setPath(path);
    recentPCounter = 0;
    points = QList<QPointF>();
    setPen();
}

void TLineItem::addPoint(const QPointF& point, bool endOfLine) {
    recentPCounter++;
    points.append(point);
    // lastLastPoint = lastPoint;
    // lastPoint = point;
    if (recentPCounter == 1) {
        if (endOfLine || TLineItem::squareDist(lastPoint, point) > 100 * 100) {
            path.lineTo(point);
            recentPCounter = 0;
            currentPoint = point;
            lastPoint = point;
            return;
        }
        lastPoint = point;
    } else if (recentPCounter == 2) {
        path.quadTo(lastPoint, point);
        recentPCounter = 0;
        lastPoint = point;
    }

    this->setPath(path);
}

void TLineItem::refresh() {
    recentPCounter = 0;
    path = QPainterPath();
    int i;
    for (i = 0; i < points.size() - 1; i++) {
        addPoint(points[i], false);
    }
    addPoint(points[i], true);
}

void TLineItem::setPen(qreal width, const QColor& color) {
    this->pen();
    QPen pen;
    pen.setWidthF(width);
    pen.setColor(color);
    pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    QAbstractGraphicsShapeItem::setPen(pen);
}

inline qreal TLineItem::squareDist(const QPointF& p1, const QPointF& p2) {
    qreal dx = p1.x() - p2.x();
    qreal dy = p1.y() - p2.y();
    return dx * dx + dy * dy;
}

QDebug operator<<(QDebug argument, const TLineItem& obj) {
    const QPen& pen = obj.pen();
    argument.nospace() << pen.widthF() << "," << pen.color() << ","
                       << "[" << obj.points << "]";
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, TLineItem& obj) {
    qreal width;
    QColor color;
    in >> width >> color >> obj.points;
    obj.setPen(width, color);
    obj.refresh();
    return in;
}

QDataStream& operator<<(QDataStream& out, const TLineItem& obj) {
    const QPen& pen = obj.pen();
    out << pen.widthF() << pen.color() << obj.points;
    return out;
}
