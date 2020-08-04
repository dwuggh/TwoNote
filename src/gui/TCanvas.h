//
// Created by dwuggh on 8/3/20.
//

#ifndef TWONOTE_TCANVAS_H
#define TWONOTE_TCANVAS_H



#include <QWidget>
#include <QMouseEvent>
#include <QPainterPath>
#include "utils/LineShape.h"
#include <QDebug>

struct Lines {
    QList<QPointF> linePoints;
    
};

class TCanvas : public QWidget {

Q_OBJECT

public:
    explicit TCanvas(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;
private:
    LineShapes lineShapes;
    LineShape currentLine;
    bool isDrawing = false;
    QPointF currentPoint;
    QPointF lastPoint;
    void paintLine(QPainter &painter, QList<QPointF> linePoints);
};


#endif //TWONOTE_TCANVAS_H
