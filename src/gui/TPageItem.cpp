#include "TPageItem.h"

TPageItem::TPageItem(int pageNumber, QPointF centralPoint,
                     QGraphicsItem* parent)
    : QGraphicsItem(parent), pageNumber(pageNumber),
      centralPoint(centralPoint) {
    pageSize = config.pageView.pageSize;
    this->centralPoint = QPointF(
        0, pageNumber * (pageSize.height() + config.pageView.verticalMargin));
    this->setPos(this->centralPoint);
    setZValue(-10);
}

QRectF TPageItem::boundingRect() const {
    qreal w = pageSize.width();
    qreal h = pageSize.height();
    return QRectF(-w / 2, -h / 2, w, h);
}

void TPageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                      QWidget* widget) {
    painter->drawRect(boundingRect());
    painter->setBrush(config.pageView.pageColor);
    painter->drawRect(boundingRect());
}

void TPageItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "propagated: press, to:" << pageNumber;
    if (event->button() == Qt::LeftButton) {
        currentLineItem = new TLineItem(event->scenePos() - centralPoint, this);
        lineItems.append(currentLineItem);
    }
}

void TPageItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // qDebug() << "propagated: move, to:" << pageNumber;
    if (event->buttons() == Qt::LeftButton) {
        currentLineItem->addPoint(event->scenePos() - centralPoint);
        // currentLineItem->addPoint(event->scenePos());
    }
}

void TPageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "propagated: release, to:" << pageNumber;
        currentLineItem->addPoint(event->scenePos() - centralPoint, true);
    }
}

QDataStream& operator>>(QDataStream& in, TPageItem& obj) {
    in >> obj.pageNumber >> obj.lineItems >> obj.pixmaps;
    obj.setPos(obj.centralPoint);
    for (TLineItem* line : obj.lineItems) {
        line->setParentItem(&obj);
    }
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPageItem& obj) {
    out << obj.pageNumber << obj.lineItems << obj.pixmaps;
    return out;
}

QDataStream& operator>>(QDataStream& in, TPageItem*& obj) {
    int pn;
    in >> pn;
    obj = new TPageItem(pn);
    qDebug() << pn << obj->centralPoint;
    in >> obj->lineItems >> obj->pixmaps;
    for (TLineItem* line : obj->lineItems) {
        line->setParentItem(obj);
    }
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPageItem* obj) {
    out << *obj;
    return out;
}
