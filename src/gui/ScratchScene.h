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

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *);

private:
  QPen pen;
  QGraphicsScene scene;
  QPointF currentPoint;
  QPointF lastPoint;
  bool isDrawing;
};

#endif
