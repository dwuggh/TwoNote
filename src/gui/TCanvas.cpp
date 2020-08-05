//
// Created by dwuggh on 8/3/20.
//

#include "TCanvas.h"

TCanvas::TCanvas(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
    lineShapes = QList<LineShape>();
    pen = QPen();
    pen.setColor(Qt::black);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(3.0);
}

void TCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        currentPoint = event->scenePos();
	lastPoint = event->scenePos();
        currentLine = LineShape();
        currentLine.append(currentPoint);
    }
    currentPath = QPainterPath();
    currentPath.moveTo(currentPoint);
    currentPathItem = this->addPath(currentPath, pen);
    currentPathItem->setPen(pen);
    qDebug() << this->sceneRect();
}

void TCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
	lastPoint = currentPoint;
        currentPoint = event->scenePos();
        currentLine.append(currentPoint);
    }
    currentPath.quadTo(lastPoint, currentPoint);
    currentPathItem->setPath(currentPath);
}

void TCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
    lineShapes.append(currentLine);
    qDebug() << currentPath;
}

void TCanvas::paintLine(QPainter &painter, QList<QPointF> linePoints) {
    QPainterPath linePaths;
    if (linePoints.length() >= 1) {
	linePaths.moveTo(linePoints[0]);
    }
    else return;

    for (int i = 1; i < linePoints.length() - 2; i ++) {
	linePaths.quadTo(linePoints[i], linePoints[i+1]);
    }

}

void TCanvas::wheelEvent(QGraphicsSceneWheelEvent *event) {
    qreal delta_y = event->delta() * 0.1;
    // this->update(QRectF());
}
