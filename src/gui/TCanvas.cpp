//
// Created by dwuggh on 8/3/20.
//

#include "TCanvas.h"

TCanvas::TCanvas(QWidget* parent) : QWidget(parent) {
    isDrawing = false;
    lineShapes = QList<LineShape>();
}

void TCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        currentPoint = event->pos();
        currentLine = LineShape();
        currentLine.append(currentPoint);
    }
    this->update();
}

void TCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        currentPoint = event->pos();
        currentLine.append(currentPoint);
    }
    this->update();
}

void TCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
    lineShapes.append(currentLine);
    this->update();
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

    painter.drawPath(linePaths);
}

void TCanvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHints(
              QPainter::Antialiasing
            | QPainter::HighQualityAntialiasing
            | QPainter::SmoothPixmapTransform
            );

   for (auto line: this->lineShapes) {
       qDebug() << line;
       line.quadPaintLine(painter);
   }
   if (this->isDrawing) {
       qDebug() << currentLine;
       currentLine.quadPaintLine(painter);
   }
//    paintLine(painter, this->currentLine.points);
}
