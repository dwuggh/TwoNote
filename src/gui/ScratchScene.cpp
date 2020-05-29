#include "ScratchScene.h"

ScratchScene::ScratchScene(QWidget* parent) : QGraphicsScene(parent) {
    isDrawing = false;
    // pen = QPen(Qt::black, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    // set pen's property
    // TODO width and color should be customizeable
    pen = QPen();
    pen.setWidth(5.0);
    pen.setColor(QColor(Qt::black));
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
        std::cout << lastPoint.x() << std::endl;
        this->addLine(QLineF(lastPoint, currentPoint), pen);

        // this->update();
    }
}

void ScratchScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        isDrawing = false;
        // lastPoint = currentPoint;
        // currentPoint = event->pos();
        // this->addLine(QLineF(lastPoint, currentPoint), pen);
    }
}
