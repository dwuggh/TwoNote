#include "ScratchScene.h"

ScratchScene::ScratchScene(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
    // TODO width and color should be customizeable
    pen = QPen();
    this->setPen();

}

void ScratchScene::setPen(QColor color, qreal width) {
    this->color = color;
    this->penWidth = width;
    pen.setColor(color);
    pen.setWidthF(width);
}

void ScratchScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true;
        currentPoint = event->scenePos();
        lastPoint = event->scenePos();
    }
}

void ScratchScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        lastPoint = currentPoint;
        currentPoint = event->scenePos();
        drawLine(lastPoint, currentPoint);

        // this->update();
    }
}

void ScratchScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
}

void ScratchScene::drawLine(QPointF p1, QPointF p2) {
    qreal r = 2.0;
    qreal x1 = p1.rx();
    qreal x2 = p2.rx();
    qreal y1 = p1.ry();
    qreal y2 = p2.ry();
    this->addEllipse(p1.rx(), p1.ry(), r, r);
    this->addEllipse(p2.rx(), p2.ry(), r, r);

    // draw path
    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    this->addPath(path);
}
