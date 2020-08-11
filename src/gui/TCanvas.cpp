//
// Created by dwuggh on 8/3/20.
//

#include "TCanvas.h"


TCanvas::TCanvas(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
    pen.setColor(config.pageView.defaultPenColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(3.0);

    // the default name is just "", need to specify file name when saving
    QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    // store in temp file
    setName(config.tempDir.absolutePath() + name);
    qDebug() << "creating new buffer:" << bufferName;
    uuid = QUuid::createUuid();

    pageSize = config.pageView.pageSize;
    // add one page at start
    currentPageNumber = newPage();
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

    QString oldName = bufferName;
    QString filename = name;
    if (QFileInfo(name).suffix() != "tnote") {
	filename = name + ".tnote";
    }
    bufferName = filename;
    file.setFileName(QFileInfo(filename).absoluteFilePath());
    return oldName;
}

void TCanvas::save() {
    qDebug() << "saving" << this->bufferName;
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *this;
    file.close();
}

void TCanvas::saveAs(const QString& name) {
    qDebug() << "saving " << this->bufferName << "as:" << name;
    file.setFileName(name);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << *this;
    file.close();
    if (bufferName == "") {
	bufferName = name;
    } else {
	file.setFileName(bufferName);
    }
}

int TCanvas::newPage() {
    int newPageIndex = pages.size();
    qDebug() << "new page for buffer:" << this->bufferName
	     << "page: " << newPageIndex;
    pages.append(TPage(newPageIndex));
    // TPage& page = pages.last();
    updateSceneRect();
    return newPageIndex;
}

void TCanvas::drawBackground(QPainter* painter, const QRectF &rect) {
    painter->setBrush(config.pageView.backgroundColor);
    painter->drawRect(rect);
    painter->setBrush(config.pageView.pageColor);
    for (auto page: pages) {
	painter->drawRect(page.pageRect());
    }
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && this->contains(event->scenePos())) {
	isDrawing = true;
	currentPoint = event->scenePos();
        qDebug() << "start at:" << currentPoint;
        qDebug() << "size:" << sceneRect();
        lastPoint = event->scenePos();
        currentLine = LineShape(pen.widthF(), pen.color());
        currentLine.append(currentPoint);

        // new path
        currentPath = QPainterPath();
        currentPath.moveTo(currentPoint);
        currentPathItem = this->addPath(currentPath, pen);
        currentPathItem->setPen(pen);

        // count page number when start drawing
        // if the line cross multiple pages, it still belong to the page
        // which has its start point.
        currentPageNumber = choosedPage(currentPoint);
        // qDebug() << this->sceneRect();
        return;
    }
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton && isDrawing && this->contains(event->scenePos())) {

	lastPoint = currentPoint;
        currentPoint = event->scenePos();
        currentLine.append(currentPoint);
        currentPath.quadTo(lastPoint, currentPoint);
        currentPathItem->setPath(currentPath);
    }
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing && this->contains(event->scenePos())) {
        isDrawing = false;
	// pages[currentPageNumber].lines.append(currentLine);
	pages[currentPageNumber].addLine(currentLine);
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

inline void TCanvas::updateSceneRect() {
    int width = pageSize.width();
    int height = pageSize.height();
    int size = pages.size();
    int margin = config.pageView.verticalMargin;
    this->setSceneRect(
		       - width / 2,
		       - height / 2 - margin / 2,
		       width,
		       size * ( height + margin )
		       );
}
void TCanvas::paintPages() {
    this->clear();
    updateSceneRect();
    for (TPage& page: this->pages) {
	qDebug() << "painting page" << page.pageNumber;
	qDebug() << "lines: " << page.sceneLineShapes();
	paintLines(page.sceneLineShapes());
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
    for (auto page: pages) {
	QRectF rect = page.pageRect();
	if (rect.contains(point)) {
	    return true;
	}
    }
    return false;
}

void TCanvas::wheelEvent(QGraphicsSceneWheelEvent *event) {
    qreal delta_y = event->delta() * 0.1;
    // this->update(QRectF());
}

QDebug operator<<(QDebug argument, const TCanvas &obj) {
    argument.nospace()
	<< "uuid: " << obj.uuid
	<< "file: " << obj.bufferName
	<< "," << obj.pages;
    return argument.space();
}

QDataStream &operator>>(QDataStream &in, TCanvas &obj) {
    in >> obj.pages;
    return in;
}

QDataStream &operator<<(QDataStream &out, const TCanvas &obj) {
    out << obj.pages;
    return out;
}

