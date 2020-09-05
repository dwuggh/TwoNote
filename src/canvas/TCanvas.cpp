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

    item         = nullptr;
    rubberBand   = nullptr;
    selectionCmd = nullptr;
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
            page->setUndoStack(undoStack);
        }
        updateSceneRect();
    }
}

QString TCanvas::setName(const QString& name) {
    QString oldName  = name;
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
    qDebug() << "new page for buffer:" << this->name << "page: " << newPageIndex;
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
            if (undoStack->canUndo()) {
                undoStack->undo();
            }
            break;
        case Qt::Key_Y:
            qDebug() << "press C-y";
            if (undoStack->canRedo()) {
                undoStack->redo();
            }
            break;
        default:;
            // pass
        }
    }
    if (event->key() == Qt::Key_Escape) {
	this->setSelectionArea(QPainterPath());
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

    /*
     * 1. is the point inside pages?
     * 2. left button or right button?
     * 3. selecting something?
     * 4. unfinished jobs?
     */

    currentPoint = event->scenePos();
    // if the point is outside of every page, perform no further action
    if (!this->contains(currentPoint)) {
        event->accept();
        return;
    }

    // if select something, move it
    groupSelection = selectedItems();
    if (event->button() == Qt::LeftButton && groupSelection.size() != 0) {
        selectionTransform = QTransform();
	setSelectionCmd();
	doingGroupSelection = true;
        return;
    }

    if (state.editState == EditState::draw && event->button() == Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }

    if (event->button() == Qt::RightButton) {
        rubberBand = new TRubberBandItem(event->scenePos());
        addItem(rubberBand);
        setSelectionArea(rubberBand->selectionArea());
	return;
    }
    QGraphicsScene::mousePressEvent(event);
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (!this->contains(event->scenePos())) {
        event->accept();
        return;
    }

    if (rubberBand && event->buttons() == Qt::RightButton) {
        rubberBand->addPoint(event->scenePos());
        setSelectionArea(rubberBand->selectionArea());
        return;
    }

    if (doingGroupSelection && event->buttons() == Qt::LeftButton) {
	
        QPointF p1 = event->lastScenePos();
        QPointF p2 = event->scenePos();
        for (auto s : groupSelection) {
            s->setTransform(QTransform::fromTranslate(p2.x() - p1.x(), p2.y() - p1.y()), true);
        }
        return;
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (!this->contains(event->scenePos())) {
        event->accept();
        return;
    }

    if (rubberBand) {
        delete rubberBand;
        rubberBand = nullptr;
        return;
    }

    if (doingGroupSelection || event->buttons() == Qt::LeftButton) {
	
        QPointF p1 = event->lastScenePos();
        QPointF p2 = event->scenePos();
        for (auto s : groupSelection) {
            s->setTransform(QTransform::fromTranslate(p2.x() - p1.x(), p2.y() - p1.y()), true);
        }
	doingGroupSelection = false;
        selectionCmd->update();
        undoStack->push(selectionCmd);
	selectionCmd = nullptr;
        return;
    }

    if (event->button() == Qt::LeftButton) {
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

inline void TCanvas::updateSceneRect() {
    qreal width  = pageSize.width();
    qreal height = pageSize.height();
    int size     = pages.size();
    qreal margin = config.pageView.verticalMargin;
    this->setSceneRect(-width / 2, -height / 2 - margin / 2, width, size * (height + margin));
}

int TCanvas::chosenPage(QPointF& scenePoint) {
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

void TCanvas::setSelectionCmd(bool refresh) {
    if (selectionCmd) {
        if (refresh) {
            qDebug() << "deleting" << selectionCmd;
            delete selectionCmd;
        } else {
            qDebug() << "detected" << selectionCmd << ", TCanvas::setSelectionCmd terminated";
            return;
        }
    }
    selectionCmd   = new TransformUndoCommand;
    for (auto s : groupSelection) {
        selectionCmd->registerItem(s);
    }
}

void TCanvas::wheelEvent(QGraphicsSceneWheelEvent* event) {
    qreal delta_y = event->delta() * 0.1;
    // this->update(QRectF());
}

QDebug operator<<(QDebug argument, const TCanvas& obj) {
    argument.nospace() << "uuid: " << obj.uuid << "file: " << obj.name << "," << obj.pages;
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
