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
#include <QDataStream>
#include <QFile>
#include <QToolButton>


class TCanvas : public QGraphicsScene {

Q_OBJECT

public:
    explicit TCanvas(QWidget *parent = nullptr);
    QToolButton *saveButton;
    QToolButton *loadButton;

public slots:
    void save();
    void load();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    void wheelEvent(QGraphicsSceneWheelEvent *) override;
    // void paintEvent(QPaintEvent *) override;
private:
    QFile file;
    LineShapes* lineShapes;
    LineShape currentLine;
    bool isDrawing = false;
    QPointF currentPoint;
    QPointF lastPoint;
    QPainterPath currentPath;
    QGraphicsPathItem* currentPathItem;
    QPen pen;
    void paintLines();
};


#endif //TWONOTE_TCANVAS_H
