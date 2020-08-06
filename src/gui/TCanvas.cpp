//
// Created by dwuggh on 8/3/20.
//

#include "TCanvas.h"

TCanvas::TCanvas(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
    lineShapes = new LineShapes;
    pen = QPen();
    pen.setColor(Qt::black);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(3.0);
    // file util
    // file.setObjectName(QString("test"));
    file.setFileName("test.tnote");
    // this->lineShapes = new LineShapes;
    // file.open();
    saveAction = new QAction;
    saveAction->setText(tr("save"));
    saveAction->setCheckable(false);
    saveAction->setChecked(false);
    loadAction = new QAction;
    loadAction->setText(tr("load"));
    loadAction->setCheckable(false);
    loadAction->setChecked(false);
    connect(saveAction, &QAction::triggered, this, &TCanvas::save);
    connect(loadAction, &QAction::triggered, this, &TCanvas::load);
}

void TCanvas::save() {
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    // const LineShapes ls = this->lineShapes;
    out << *this->lineShapes;
    file.close();
    // this->saveButton->setChecked(false);
}

void TCanvas::load() {
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    
    delete this->lineShapes;
    this->lineShapes = new LineShapes();
    in >> *this->lineShapes;
    file.close();
    this->paintLines();
    // this->loadButton->setChecked(false);
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        currentPoint = event->scenePos();
	lastPoint = event->scenePos();
        currentLine = LineShape(pen.widthF(), pen.color());
        currentLine.append(currentPoint);
        currentPath = QPainterPath();
        currentPath.moveTo(currentPoint);
        currentPathItem = this->addPath(currentPath, pen);
        currentPathItem->setPen(pen);
        // qDebug() << this->sceneRect();
    }
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
	lastPoint = currentPoint;
        currentPoint = event->scenePos();
        currentLine.append(currentPoint);
        currentPath.quadTo(lastPoint, currentPoint);
        currentPathItem->setPath(currentPath);
    }
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
        lineShapes->append(currentLine);
        qDebug() << currentLine;
    }
}

void TCanvas::paintLines() {
    this->clear();

    for (auto line: *this->lineShapes) {
	pen = QPen();
	pen.setColor(line.color);
	pen.setWidthF(line.width);
	currentPath = QPainterPath();
	auto points = line.points;
	currentPath.moveTo(points[0]);
	for (int i = 1; i <points.length() - 2; i++) {
	    currentPath.quadTo(points[i], points[i + 1]);
	}
	this->addPath(currentPath, pen);
    }

}

void TCanvas::wheelEvent(QGraphicsSceneWheelEvent *event) {
    qreal delta_y = event->delta() * 0.1;
    // this->update(QRectF());
}
