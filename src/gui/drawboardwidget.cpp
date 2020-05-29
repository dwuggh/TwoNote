#include "drawboardwidget.h"
// #include <qnamespace.h>
// #include <qpainter.h>
// #include <qpen.h>
// #include <qpoint.h>
// #include <qwidget.h>

DrawBoardWidget::DrawBoardWidget(QWidget *parent) : QWidget(parent) {
    isDrawing = false;
    image = QImage(700, 500, QImage::Format_RGB32);
    bgColor = qRgb(255, 255, 255);
    image.fill(bgColor);

    penColor = Qt::black;
    penWidth = 1.0;
}

void DrawBoardWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.drawImage(0, 0, image);
}

void DrawBoardWidget::mousePressEvent(QMouseEvent *event) {
    if ( event->button() == Qt::LeftButton ) {
        currentPoint = event->pos();
        lastPoint = currentPoint;
        // QPainter painter(this);
        // painter.drawPoint(currentPoint);
        isDrawing = true;
    }
}

void DrawBoardWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
}

void DrawBoardWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        lastPoint = currentPoint;
        currentPoint = event->pos();
        // QPainter painter(this);
        // painter.drawPoint(currentPoint);
        // painter.drawLine(lastPoint, currentPoint);
        drawLine(lastPoint, currentPoint);
    }
}

void DrawBoardWidget::drawLine(const QPoint &p1, const QPoint &p2) {
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(p1, p2);

    // avoid complete update
    int rad = penWidth/2 + 2;
    update(QRect(p1, p2).normalized().adjusted(-rad, -rad, rad, rad));
}
