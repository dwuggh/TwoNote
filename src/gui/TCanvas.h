//
// Created by dwuggh on 8/3/20.
//

#ifndef TWONOTE_TCANVAS_H
#define TWONOTE_TCANVAS_H



#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include "utils/LineShape.h"
#include <QDebug>


class TCanvas : public QGraphicsScene {

Q_OBJECT

public:
    explicit TCanvas(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    void wheelEvent(QGraphicsSceneWheelEvent *) override;
    // void paintEvent(QPaintEvent *) override;
private:
    LineShapes lineShapes;
    LineShape currentLine;
    bool isDrawing = false;
    QPointF currentPoint;
    QPointF lastPoint;
    QPainterPath currentPath;
    QGraphicsPathItem* currentPathItem;
    QPen pen;
    void paintLine(QPainter &painter, QList<QPointF> linePoints);
};


#endif //TWONOTE_TCANVAS_H
