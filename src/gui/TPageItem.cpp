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
    setAcceptDrops(true);
}

TCanvas* TPageItem::canvas() {
    return static_cast<TCanvas*>(scene());
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
    // qDebug() << "propagated: press, to:" << pageNumber;
    
    if (event->button() == Qt::LeftButton) {
        QPointF p = event->scenePos() - centralPoint;
        qDebug() << p;
        currentLineItem = new TLineItem(p, this);
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
        // qDebug() << "propagated: release, to:" << pageNumber;
        currentLineItem->addPoint(event->scenePos() - centralPoint, true);
    }
}

QDataStream& operator>>(QDataStream& in, TPageItem& obj) {
    in >> obj.pageNumber >> obj.lineItems >> obj.pixmapItems;
    obj.setPos(obj.centralPoint);
    for (TLineItem* line : obj.lineItems) {
        line->setParentItem(&obj);
    }
    for (TPixmapItem* pixmap : obj.pixmapItems) {
        pixmap->setParentItem(&obj);
    }
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPageItem& obj) {
    out << obj.pageNumber << obj.lineItems << obj.pixmapItems;
    return out;
}

QDataStream& operator>>(QDataStream& in, TPageItem*& obj) {
    int pn;
    in >> pn;
    obj = new TPageItem(pn);
    qDebug() << pn << obj->centralPoint;
    in >> obj->lineItems >> obj->pixmapItems;
    for (TLineItem* line : obj->lineItems) {
        line->setParentItem(obj);
    }
    for (TPixmapItem* pixmap : obj->pixmapItems) {
        pixmap->setParentItem(obj);
    }
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPageItem* obj) {
    out << *obj;
    return out;
}

void TPageItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    qDebug() << "TPage::dragEnterEvent";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        // the dropEvent's event->scenePos is somewhat buggy, don't know why
        QPointF p = event->scenePos() - centralPoint;
        if (contains(p)) {
            currentPoint = p;
        }
    }
}

void TPageItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasUrls()) {
        // qDebug() << "TPage::dragmoveEvent";
        event->acceptProposedAction();
        event->setAccepted(true);
        QPointF p = event->scenePos() - centralPoint;
        // qDebug() << p << event->pos() - centralPoint;
        if (contains(p)) {
            currentPoint = p;
        }
    }
}

void TPageItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasUrls()) {
        qDebug() << "TPage::dragLeaveEvent";
        event->acceptProposedAction();
        event->setAccepted(true);
    }
}

void TPageItem::dropEvent(QGraphicsSceneDragDropEvent* event) {
    qDebug() << "TPage::dropEvent";
    const QMimeData* mimeData = event->mimeData();
    if (!mimeData->hasUrls()) {
        event->ignore();
        return;
    }
    event->accept();
    for (QUrl& url : mimeData->urls()) {
        if (!url.isLocalFile())
            continue;
        qDebug() << "image:" << url;
        QPixmap pixmap(url.toLocalFile());
        QSize pSize = pixmap.size();
        QPointF p = event->scenePos() - centralPoint;
        qDebug() << p << currentPoint;
        p = currentPoint;
        qreal factor = 1.0;
        if (!contains(QPointF(p.x() + pSize.width(), p.y() + pSize.height()))) {
            factor = (pageSize.width() / 2 - p.x()) / pSize.width();
        }
	pixmapItems.append(new TPixmapItem(pixmap, p, factor, factor, this));
    }
}
