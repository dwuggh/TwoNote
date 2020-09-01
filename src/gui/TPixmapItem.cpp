//
// Created by dwuggh on 9/1/20.
//

#include "TPixmapItem.h"

TPixmapItem::TPixmapItem(const QPixmap& pixmap, QPointF position, qreal scaleX,
                         qreal scaleY, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent), scaleX(scaleX), scaleY(scaleY) {
    setTransformationMode(Qt::SmoothTransformation);
    setPos(position);
    setTransform(QTransform::fromScale(scaleX, scaleY));
}

QDataStream& operator>>(QDataStream& in, TPixmapItem& obj) {
    QPixmap pixmap;
    QPointF pos;
    in >> pixmap >> pos;
    obj.setPixmap(pixmap);
    obj.setPos(pos);
    in >> obj.scaleX >> obj.scaleY;
    obj.setTransformationMode(Qt::SmoothTransformation);
    obj.setTransform(QTransform::fromScale(obj.scaleX, obj.scaleY));
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPixmapItem& obj) {
    out << obj.pixmap() << obj.pos() << obj.scaleX << obj.scaleY;
    return out;
}

QDataStream& operator>>(QDataStream& in, TPixmapItem*& obj) {
    QPixmap pixmap;
    QPointF pos;
    qreal sx, sy;
    in >> pixmap >> pos >> sx >> sy;
    obj = new TPixmapItem(pixmap, pos, sx, sy);
    // in >> *obj;
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPixmapItem* obj) {
    out << *obj;
    return out;
}

void TPixmapItem::setScale(qreal sx, qreal sy) {
    qDebug() << "set scale to:" << sx << sy;
    scaleX = sx;
    scaleY = sy;
    setTransform(QTransform::fromScale(sx, sy));
}
