#include "TPixmapItem.h"

TPixmapItem::TPixmapItem(const QPixmap& pixmap, QPointF position, qreal scaleX, qreal scaleY,
                         QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent) {
    setTransformationMode(Qt::SmoothTransformation);
    setPos(position);
    beforeResizePos       = position;
    beforeResizeTransform = QTransform::fromScale(scaleX, scaleY);
    setTransform(beforeResizeTransform);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setZValue(-10);
    setupHandles();
    setAcceptHoverEvents(true);
}

QDataStream& operator>>(QDataStream& in, TPixmapItem& obj) {
    QPixmap pixmap;
    QPointF pos;
    QTransform transform;
    in >> pixmap >> pos >> transform;
    obj.setPixmap(pixmap);
    obj.beforeResizePos = pos;
    obj.setPos(pos);
    obj.beforeResizeTransform = transform;
    obj.setTransform(transform);
    obj.setTransformationMode(Qt::SmoothTransformation);
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPixmapItem& obj) {
    out << obj.pixmap() << obj.pos() << obj.transform();
    return out;
}

QDataStream& operator>>(QDataStream& in, TPixmapItem*& obj) {
    QPixmap pixmap;
    QPointF pos;
    QTransform transform;
    in >> pixmap >> pos >> transform;
    obj                        = new TPixmapItem(pixmap, pos);
    obj->beforeResizeTransform = transform;
    obj->beforeResizePos       = pos;
    obj->setTransform(transform);
    // in >> *obj;
    return in;
}

QDataStream& operator<<(QDataStream& out, const TPixmapItem* obj) {
    out << *obj;
    return out;
}

inline void TPixmapItem::setScale(qreal sx, qreal sy, bool composite) {
    setTransform(QTransform::fromScale(sx, sy), composite);
}

void TPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    const QPointF& p = event->pos();
    if (selectHandle(p) != noHandle) {
        beforeResizeTransform = transform();
        beforeResizePos       = pos();
        interactiveResize(p);
    } else {
        QGraphicsPixmapItem::mousePressEvent(event);
    }
}

void TPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (selectedHandle != noHandle) {
        const QPointF& p = event->pos();
        interactiveResize(p);
    } else {
        QGraphicsPixmapItem::mouseMoveEvent(event);
    }
}

void TPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    auto* cmd = new ResizeUndoCommand();
    cmd->addItem(this, beforeResizePos, beforeResizeTransform);
    undoStack->push(cmd);
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void TPixmapItem::setupHandles() {
    const QRectF& b = boundingRect();
    qreal s         = 10.0;
    qreal w         = b.width();
    qreal h         = b.height();

    handles[handleTopLeft]      = QRectF(b.left(), b.top(), s, s);
    handles[handleTopMiddle]    = QRectF(b.left() + s, b.top(), w - 2 * s, s);
    handles[handleTopRight]     = QRectF(b.right() - s, b.top(), s, s);
    handles[handleMiddleLeft]   = QRectF(b.left(), b.top() - s, s, h - 2 * s);
    handles[handleMiddleRight]  = QRectF(b.right() - s, b.top() - s, s, h - 2 * s);
    handles[handleBottomLeft]   = QRectF(b.left(), b.bottom() - s, s, s);
    handles[handleBottomMiddle] = QRectF(b.left() - s, b.bottom() - s, w - 2 * s, s);
    handles[handleBottomRight]  = QRectF(b.right() - s, b.bottom() - s, s, s);
}

HandleType TPixmapItem::selectHandle(const QPointF& p) {
    for (auto it = handles.begin(); it != handles.end(); it++) {
        if (it.value().contains(p)) {
            selectedHandle = it.key();
            return selectedHandle;
        }
    }
    selectedHandle = noHandle;
    return noHandle;
}

void TPixmapItem::interactiveResize(const QPointF& pos) {
    this->prepareGeometryChange();
    const QRectF& b = boundingRect();
    qreal w         = b.width();
    qreal h         = b.height();
    qreal sx = 1, sy = 1;
    QPointF newPos = this->pos();
    switch (selectedHandle) {
    case handleTopLeft:
        newPos = mapToParent(pos);
        sx     = (w - pos.x()) / w;
        sy     = (h - pos.y()) / h;
        break;
    case handleTopMiddle:
        newPos.setY(mapToParent(pos).y());
        sx = 1;
        sy = (h - pos.y()) / h;
        break;
    case handleTopRight:
        newPos.setY(mapToParent(pos).y());
        sx = pos.x() / w;
        sy = (h - pos.y()) / h;
        break;
    case handleMiddleLeft:
        newPos.setX(mapToParent(pos).x());
        sx = (w - pos.x()) / w;
        sy = 1;
        break;
    case handleMiddleRight:
        sx = pos.x() / w;
        sy = 1;
        break;
    case handleBottomLeft:
        newPos.setX(mapToParent(pos).x());
        sx = (w - pos.x()) / w;
        sy = pos.y() / h;
        break;
    case handleBottomMiddle:
        sx = 1;
        sy = pos.y() / h;
        break;
    case handleBottomRight:
        sx = pos.x() / w;
        sy = pos.y() / h;
        break;
    case noHandle:
        return;
    }
    setPos(newPos);
    setScale(sx, sy, true);
}

void TPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event) {
    if (selectHandle(event->pos()) != noHandle) {
        setCursor(QCursor(Qt::SizeHorCursor));
    } else {
        setCursor(QCursor(Qt::ArrowCursor));
    }
    switch (selectHandle(event->pos())) {
    case handleTopLeft:
        setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case handleTopMiddle:
        setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case handleTopRight:
        setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case handleMiddleLeft:
        setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case handleMiddleRight:
        setCursor(QCursor(Qt::SizeHorCursor));
        break;
    case handleBottomLeft:
        setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
    case handleBottomMiddle:
        setCursor(QCursor(Qt::SizeVerCursor));
        break;
    case handleBottomRight:
        setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
    case noHandle:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    }
    QGraphicsPixmapItem::hoverMoveEvent(event);
}

void TPixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}

void TPixmapItem::setUndoStack(QSharedPointer<QUndoStack>& undoStack) {
    this->undoStack = undoStack;
}
