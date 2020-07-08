#ifndef SCRATCHSCENE_H
#define SCRATCHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QLineF>
#include <QMoveEvent>
#include <QPointF>
#include <QRectF>
#include <QGraphicsLineItem>
#include <iostream>

// #include <QtWidgets>

/* QT_BEGIN_NAMESPACE */

/* namespace Scratch { */
/*     class ScratchScene; */
/* } */

/* QT_END_NAMESPACE */

class ScratchScene : public QGraphicsScene {

    Q_OBJECT

public:
    ScratchScene(QWidget *parent = nullptr);

    void setPen(QColor color = QColor(Qt::black), qreal width = 2.0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
    QPen pen;
    qreal penWidth;
    QColor color;
    QGraphicsScene scene;
    QPointF currentPoint;
    QPointF lastPoint;
    bool isDrawing;

    void drawLine(QPointF, QPointF);
};

#endif
