#include "TCanvas.h"

TCanvas::TCanvas(QWidget* parent) : QGraphicsScene(parent) {
    pen.setColor(config.pageView.defaultPenColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(3.0);

    // the default name is just "", need to specify file name when saving
    QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    // store in temp file
    setName(config.tempDir.absolutePath() + "/" + name);
    qDebug() << "creating new buffer:" << name;
    uuid = QUuid::createUuid();

    pageSize = config.pageView.pageSize;

    // undo/redo utility
    undoStack = QSharedPointer<QUndoStack>(new QUndoStack(this));
    // add one page at start
    currentPageNumber = newPage();

    item = nullptr;
}

TCanvas::TCanvas(const QString& name, QWidget* parent) : TCanvas(parent) {
    setName(name);
    // if file exists, load its content from disk
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> pages;
        file.close();
        for (TPage* page : pages) {
            this->addItem(page);
        }
        updateSceneRect();
    }
}

QString TCanvas::setName(const QString& name) {
    QString oldName = name;
    QString filename = name;
    if (QFileInfo(name).suffix() != "tnote") {
        filename = name + ".tnote";
    }
    this->name = filename;
    file.setFileName(QFileInfo(filename).absoluteFilePath());
    return oldName;
}

void TCanvas::save() {
    qDebug() << "saving" << this->name;
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *this;
    file.close();
}

void TCanvas::saveAs(const QString& name) {
    qDebug() << "saving " << this->name << "as:" << name;
    file.setFileName(name);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *this;
    file.close();
    if (this->name.startsWith("/tmp") || this->name == "") {
        this->name = name;
    } else {
        file.setFileName(name);
    }
}

int TCanvas::newPage() {
    int newPageIndex = pages.size();
    qDebug() << "new page for buffer:" << this->name
             << "page: " << newPageIndex;
    TPage* page = new TPage(newPageIndex);
    pages.append(page);
    this->addItem(page);
    page->setUndoStack(undoStack);
    // TPage& page = pages.last();
    updateSceneRect();
    return newPageIndex;
}

void TCanvas::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->setBrush(config.pageView.backgroundColor);
    painter->drawRect(rect);
}

void TCanvas::keyPressEvent(QKeyEvent* event) {
    // if (event->modifiers() == Qt::ControlModifier) {
    // 	switch (event->key())
    // }
    qDebug() << "press key" << event->key();
    if (event->modifiers() == Qt::ControlModifier) {
	switch (event->key()) {
	case Qt::Key_Z:
	    qDebug() << "press C-z";
	    qDebug() << undoStack->canUndo();
	    if (undoStack->canUndo()) undoStack->undo();
	    break;
	case Qt::Key_Y:
	    qDebug() << "press C-y";
	    if (undoStack->canRedo()) undoStack->redo();
	    break;
	default: ;
	    // pass
	}
    }
    // if (event->matches(QKeySequence::Undo)) {
    // 	undoStack->undo();
    // }
    // else if (event->matches(QKeySequence::Redo)) {
    // 	undoStack->redo();
    // }
    QGraphicsScene::keyPressEvent(event);
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    currentPoint = event->scenePos();
    QGraphicsScene::mousePressEvent(event);
    if (!this->contains(currentPoint)) {
        event->accept();
        return;
    }
    if (state.editState == EditState::draw && event->button() == Qt::LeftButton) {
        for (QGraphicsView* view : this->views()) {
            view->viewport()->setCursor(Qt::BlankCursor);
        }
        return;
    }
    // add typed text
    else if (state.editState == EditState::type && event->button() == Qt::LeftButton) {

        qDebug() << this->focusItem() << this->items().length();
        QGraphicsItem* newItem =
            itemAt(currentPoint, QTransform::fromScale(1, 1));
        qDebug() << newItem;
        if (static_cast<QGraphicsTextItem*>(newItem)) {
            setFocusItem(newItem);
            item = static_cast<QGraphicsTextItem*>(newItem);
        } else {
            if (item && item->document()->isEmpty()) {
                delete item;
            }
            item = new QGraphicsTextItem;
            item->setDocument(new QTextDocument("", item));
            // item->setTextCursor(QTextCursor());
            item->setPos(event->scenePos());
            item->setTextInteractionFlags(Qt::TextEditorInteraction);
            addItem(item);
            setFocusItem(item);
        }
    } else {
    }
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseMoveEvent(event);
    if (!this->contains(event->scenePos())) {
        event->accept();
        return;
    }
    if (event->buttons() == Qt::LeftButton) {
        return;
    }
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsScene::mouseReleaseEvent(event);
    if (!this->contains(event->scenePos())) {
        event->accept();
        return;
    }
    if (event->button() == Qt::LeftButton) {
        for (QGraphicsView* view : this->views()) {
            view->viewport()->setCursor(Qt::ArrowCursor);
        }
        return;
    }
}

void TCanvas::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    QGraphicsScene::dragEnterEvent(event);
}

void TCanvas::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    QGraphicsScene::dragMoveEvent(event);
}

void TCanvas::dropEvent(QGraphicsSceneDragDropEvent* event) {
    QGraphicsScene::dropEvent(event);
}

inline void TCanvas::updateSceneRect() {
    int width = pageSize.width();
    int height = pageSize.height();
    int size = pages.size();
    int margin = config.pageView.verticalMargin;
    this->setSceneRect(-width / 2, -height / 2 - margin / 2, width,
                       size * (height + margin));
}

int TCanvas::choosedPage(QPointF& scenePoint) {
    for (int page = 0; true; page++) {
        if (page * pageSize.height() > qAbs(scenePoint.y())) {
            return page - 1;
        }
    }
}

bool TCanvas::contains(const QPointF& point) const {
    for (auto page : pages) {
        QRectF rect = page->boundingRect();
        if (rect.contains(point)) {
            return true;
        }
    }
    return false;
}

void TCanvas::wheelEvent(QGraphicsSceneWheelEvent* event) {
    qreal delta_y = event->delta() * 0.1;
    // this->update(QRectF());
}

QDebug operator<<(QDebug argument, const TCanvas& obj) {
    argument.nospace() << "uuid: " << obj.uuid << "file: " << obj.name << ","
                       << obj.pages;
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, TCanvas& obj) {
    in >> obj.pages;
    return in;
}

QDataStream& operator<<(QDataStream& out, const TCanvas& obj) {
    out << obj.pages;
    return out;
}
