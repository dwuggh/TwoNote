#include "TLineItem.h"

TLineItem::TLineItem(const QPointF& point, QGraphicsItem* parent) : QGraphicsPathItem(parent) {
    path           = QPainterPath();
    points         = QList<QPointF>();
    recentPCounter = 0;
    path.moveTo(point);
    setPath(path);
    this->setPen(state.pen);
    setFlag(ItemIsSelectable);
    setZValue(0);
}

void TLineItem::addPoint(const QPointF& point, bool endOfLine) {
    if (lastPoint == point) return;
    recentPCounter++;
    points.append(point);
    if (recentPCounter == 1) {
	qreal dist = TLineItem::L1Dist(lastPoint, point);
        if (endOfLine || dist > 1000 || dist < 10) {
            path.lineTo(point);
            recentPCounter = 0;
            lastPoint      = point;
	    setPath(path);
            return;
        }
        lastPoint = point;
    } else if (recentPCounter == 2) {
        path.quadTo(lastPoint, point);
        recentPCounter = 0;
        lastPoint      = point;
    }

    this->setPath(path);
}

void TLineItem::refresh() {
    qDebug() << this;
    recentPCounter = 0;
    path           = QPainterPath();
    path.moveTo(points[0]);
    int i;
    qDebug() << points.size();
    QList<QPointF> points = this->points;
    this->points          = QList<QPointF>();
    for (i = 0; i < points.size() - 1; i++) {
        addPoint(points[i], false);
    }
    addPoint(points[i], true);
}

inline qreal TLineItem::squareDist(const QPointF& p1, const QPointF& p2) {
    qreal dx = p1.x() - p2.x();
    qreal dy = p1.y() - p2.y();
    return dx * dx + dy * dy;
}

inline qreal TLineItem::L1Dist(const QPointF& p1, const QPointF& p2) {
    qreal dx = p1.x() - p2.x();
    dx       = dx > 0 ? dx : -dx;
    qreal dy = p1.y() - p2.y();
    dy       = dy > 0 ? dy : -dy;
    return dx + dy;
}

QDebug operator<<(QDebug argument, const TLineItem& obj) {
    const QPen& pen = obj.pen();
    argument.nospace() << pen.widthF() << "," << pen.color() << ","
                       << "[" << obj.points << "]";
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, TLineItem& obj) {
    QPen pen;
    QPointF pos;
    QTransform transform;
    in >> pen >> pos >> transform >> obj.points;
    obj.setPen(pen);
    obj.setPos(pos);
    obj.setTransform(transform);
    obj.refresh();
    return in;
}

QDataStream& operator<<(QDataStream& out, const TLineItem& obj) {
    const QPen& pen = obj.pen();
    out << obj.pen() << obj.pos() << obj.transform() << obj.points;
    return out;
}

QDataStream& operator>>(QDataStream& in, TLineItem*& obj) {
    obj = new TLineItem;
    in >> *obj;
    qDebug() << obj;
    return in;
}

QDataStream& operator<<(QDataStream& out, const TLineItem* obj) {
    out << *obj;
    return out;
}
