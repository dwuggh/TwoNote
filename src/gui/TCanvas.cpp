#include "TCanvas.h"

TCanvas::TCanvas(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
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
    // add one page at start
    currentPageNumber = newPage();

    state = EditState::draw;
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
        this->paintPages();
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
    pages.append(TPage(newPageIndex));
    // TPage& page = pages.last();
    updateSceneRect();
    return newPageIndex;
}

void TCanvas::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->setBrush(config.pageView.backgroundColor);
    painter->drawRect(rect);
    painter->setBrush(config.pageView.pageColor);
    for (auto page : pages) {
        painter->drawRect(page.pageRect());
    }
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    currentPoint = event->scenePos();
    if (!this->contains(currentPoint))
        return;
    if (state == EditState::draw && event->button() == Qt::LeftButton) {
        for (QGraphicsView* view : this->views()) {
            view->viewport()->setCursor(Qt::BlankCursor);
        }
        isDrawing = true;
        qDebug() << "start at:" << currentPoint;
        lastPoint = event->scenePos();
        currentLine = LineShape(pen.widthF(), pen.color());
        currentLine.append(currentPoint);

        // new path
        currentPath = QPainterPath();
        currentPath.moveTo(currentPoint);
        currentPathItem = this->addPath(currentPath, pen);
        currentPathItem->setPen(pen);
        currentPathItem->setFlag(QGraphicsItem::ItemIsMovable);
        currentPathItem->setFlag(QGraphicsItem::ItemIsSelectable);

        // count page number when start drawing
        // if the line cross multiple pages, it still belong to the page
        // which has its start point.
        currentPageNumber = choosedPage(currentPoint);
        // qDebug() << this->sceneRect();
        return;
    }
    // add typed text
    else if (state == EditState::type && event->button() == Qt::LeftButton) {

        qDebug() << this->focusItem() << this->items().length();
        QGraphicsItem* newItem =
            itemAt(currentPoint, QTransform::fromScale(1, 1));
        qDebug() << newItem;
        if (dynamic_cast<QGraphicsTextItem*>(newItem)) {
            setFocusItem(newItem);
            item = dynamic_cast<QGraphicsTextItem*>(newItem);
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
    }
    else {
      
    }
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (!this->contains(event->scenePos()))
        return;
    if (isDrawing && event->buttons() == Qt::LeftButton) {

        lastPoint = currentPoint;
        currentPoint = event->scenePos();
        currentLine.append(currentPoint);
        currentPath.quadTo(lastPoint, currentPoint);
        currentPathItem->setPath(currentPath);
    }
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (!this->contains(event->scenePos()))
        return;
    if (isDrawing && event->button() == Qt::LeftButton) {
        isDrawing = false;
        // pages[currentPageNumber].lines.append(currentLine);
        pages[currentPageNumber].addLine(currentLine);
        for (QGraphicsView* view : this->views()) {
            view->viewport()->setCursor(Qt::ArrowCursor);
        }
    }
}

void TCanvas::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    qDebug() << "Scene::dragEnterEvent";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        if (contains(event->scenePos())) {
            currentPoint = event->scenePos();
        }
    }
}

void TCanvas::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    // qDebug() << "Scene::dragmoveEvent";
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        // the dropEvent's event->scenePos is somewhat buggy, don't know why
        if (contains(event->scenePos())) {
            currentPoint = event->scenePos();
        }
    }
}

void TCanvas::dropEvent(QGraphicsSceneDragDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();
    // qDebug() << mimeData->hasImage();
    if (!mimeData->hasUrls()) {
        event->setAccepted(false);
        return;
    }
    event->setAccepted(true);
    for (QUrl& url : mimeData->urls()) {
        qDebug() << "dropping image: " << url;
        if (!url.isLocalFile())
            continue;

        QPixmap pixmap(url.toLocalFile());
        QSize pSize = pixmap.size();
        // if the image is too large, crop it
        QGraphicsPixmapItem* pixmapItem = addPixmap(pixmap);
        pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        pixmapItem->setFlags(QGraphicsItem::ItemIsSelectable |
                             QGraphicsItem::ItemIsMovable |
			     QGraphicsItem::ItemIsFocusable);
        // pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setPos(currentPoint);
        qreal factor = 1;
        if (!contains(QPointF(currentPoint.x() + pSize.width(),
                              currentPoint.y() + pSize.height()))) {
            factor = (pageSize.width() / 2 - currentPoint.x()) / pSize.width();
            pixmapItem->setTransform(QTransform::fromScale(factor, factor));
        }
        qDebug() << factor;
        pages[currentPageNumber].addPixmap(pixmap, currentPoint, factor,
                                           factor);
    }
}

void TCanvas::paintLines(const LineShapes lines) {
    for (LineShape line : lines) {
        currentPath = QPainterPath();
        QList<QPointF> points = line.points;
        currentPath.moveTo(points[0]);
        for (int i = 1; i < points.length() - 2; i++) {
            currentPath.quadTo(points[i], points[i + 1]);
        }
        this->addPath(currentPath, line.pen);
    }
}

void TCanvas::paintPixmaps(const QList<PixmapData> pixmaps) {
    for (auto pixmap : pixmaps) {
        QGraphicsPixmapItem* pixmapItem = addPixmap(pixmap.pixmap);
        pixmapItem->setTransformationMode(Qt::SmoothTransformation);
        pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setPos(pixmap.position);
        pixmapItem->setTransform(
            QTransform::fromScale(pixmap.scaleX, pixmap.scaleY));
    }
}

inline void TCanvas::updateSceneRect() {
    int width = pageSize.width();
    int height = pageSize.height();
    int size = pages.size();
    int margin = config.pageView.verticalMargin;
    this->setSceneRect(-width / 2, -height / 2 - margin / 2, width,
                       size * (height + margin));
}
void TCanvas::paintPages() {
    this->clear();
    updateSceneRect();
    for (TPage& page : this->pages) {
        qDebug() << "painting page" << page.pageNumber;
        qDebug() << "lines: " << page.sceneLineShapes();
        qDebug() << "pixmaps: " << page.pixmaps;
        paintLines(page.sceneLineShapes());
        paintPixmaps(page.pixmaps);
    }
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
        QRectF rect = page.pageRect();
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
