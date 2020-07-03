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
        // std::cout << event->scenePos().x() << ' ' << event->scenePos().y() << std::endl;
        // std::cout << lastPoint.x() << std::endl;
        this->addLine(QLineF(lastPoint, currentPoint), pen);

        // this->update();
    }
}

void ScratchScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
    }
}
