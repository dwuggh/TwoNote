#include "TPage.h"

TPage::TPage(int pageNumber, QPointF centralPoint, QGraphicsItem* parent)
    : QGraphicsItem(parent), pageNumber(pageNumber),
      centralPoint(centralPoint) {
    pageSize = config.pageView.pageSize;
    this->centralPoint = QPointF(
        0, pageNumber * (pageSize.height() + config.pageView.verticalMargin));
    this->setPos(this->centralPoint);
    setZValue(-10);
    setAcceptDrops(true);
}

QRectF TPage::boundingRect() const {
    qreal w = pageSize.width();
    qreal h = pageSize.height();
    return QRectF(-w / 2, -h / 2, w, h);
}

void TPage::setUndoStack(QSharedPointer<QUndoStack> undoStack) {
    this->undoStack = undoStack;
}

void TPage::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                  QWidget* widget) {
    painter->drawRect(boundingRect());
    painter->setBrush(config.pageView.pageColor);
    painter->drawRect(boundingRect());
}

void TPage::addItem(QGraphicsItem* item) {
    item->setParentItem(this);
    if (TLineItem* line = dynamic_cast<TLineItem*>(item)) {
        lineItems.append(line);
        return;
    }
    if (TPixmapItem* pixmap = dynamic_cast<TPixmapItem*>(item)) {
        pixmapItems.append(pixmap);
        return;
    }
    if (QGraphicsTextItem* text = dynamic_cast<QGraphicsTextItem*>(item)) {
        return;
    }
}

void TPage::removeItem(QGraphicsItem* item) {
    item->setParentItem(nullptr);
    scene()->removeItem(item);
    if (TLineItem* line = dynamic_cast<TLineItem*>(item)) {
        lineItems.removeOne(line);
        return;
    }
    if (TPixmapItem* pixmap = dynamic_cast<TPixmapItem*>(item)) {
        pixmapItems.removeOne(pixmap);
        return;
    }
    if (QGraphicsTextItem* text = dynamic_cast<QGraphicsTextItem*>(item)) {
        return;
    }
}

void TPage::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // qDebug() << "propagated: press, to:" << pageNumber;
    QPointF p = event->scenePos() - centralPoint;
    switch (state.editState) {
    case EditState::draw:
        if (event->button() == Qt::LeftButton) {
            qDebug() << p;
            currentLineItem = new TLineItem(p, this);
            addItem(currentLineItem);
            auto command = new AddItemCommand(currentLineItem, this);
            undoStack->push(command);
        }
        break;
    case EditState::view:
        event->accept();
        break;
    case EditState::type:
        // something
        event->accept();
        for (auto item : childItems()) {
            if (item->contains(p)) {
                QGraphicsTextItem* text = static_cast<QGraphicsTextItem*>(item);
                if (text) {
                    text->setTextInteractionFlags(Qt::TextEditorInteraction);
                    scene()->setFocusItem(text);
                    return;
                }
            }
        }
        QGraphicsTextItem* text = new QGraphicsTextItem(this);
        undoStack->push(new AddItemCommand(text, this));
        text->setDocument(new QTextDocument("", text));
        text->setPos(p);
        text->setTextInteractionFlags(Qt::TextEditorInteraction);
        scene()->setFocusItem(text);
        break;
    }
}

void TPage::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // qDebug() << "propagated: move, to:" << pageNumber;
    switch (state.editState) {
    case EditState::draw:
        if (event->buttons() == Qt::LeftButton) {
            currentLineItem->addPoint(event->scenePos() - centralPoint);
        }
        qDebug() << event->screenPos() << event->scenePos();
        break;
    case EditState::view:
        event->accept();
        break;
    case EditState::type:
        event->accept();
        break;
    }
}

void TPage::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    switch (state.editState) {
    case EditState::draw:
        if (event->button() == Qt::LeftButton) {
            currentLineItem->addPoint(event->scenePos() - centralPoint, true);
        }
        break;
    case EditState::view:
        event->accept();
        break;
    case EditState::type:
        event->accept();
        break;
    }
}

QDataStream& operator>>(QDataStream& in, TPage& obj) {
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

QDataStream& operator<<(QDataStream& out, const TPage& obj) {
    out << obj.pageNumber << obj.lineItems << obj.pixmapItems;
    return out;
}

QDataStream& operator>>(QDataStream& in, TPage*& obj) {
    int pn;
    in >> pn;
    obj = new TPage(pn);
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

QDataStream& operator<<(QDataStream& out, const TPage* obj) {
    out << *obj;
    return out;
}

void TPage::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
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

void TPage::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
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

void TPage::dragLeaveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasUrls()) {
        qDebug() << "TPage::dragLeaveEvent";
        event->acceptProposedAction();
        event->setAccepted(true);
    }
}

void TPage::dropEvent(QGraphicsSceneDragDropEvent* event) {
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
        TPixmapItem* pixmapItem =
            new TPixmapItem(pixmap, p, factor, factor, this);
        pixmapItems.append(pixmapItem);
        undoStack->push(new AddItemCommand(pixmapItem, this));
    }
}

AddItemCommand::AddItemCommand(QGraphicsItem* item, TPage* page,
                               QUndoCommand* parent)
    : QUndoCommand(parent) {
    this->item = item;
    this->page = page;
    qDebug() << "constructing" << item << page;
}

void AddItemCommand::undo() { page->removeItem(item); }
void AddItemCommand::redo() { page->addItem(item); }
