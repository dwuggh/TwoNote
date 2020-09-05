#pragma once

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPen>
#include <QPointF>
#include "TLineItem.h"

enum AreaType { Rect, Any };

class TRubberBandItem : public TLineItem {
  public:
    explicit TRubberBandItem(const QPointF& start, AreaType type = AreaType::Rect,
                             QGraphicsItem* parent = nullptr);

    AreaType areaType;

    void addPoint(const QPointF& point, bool endOfLine = false) override;
    const QPainterPath& selectionArea();

  private:
    QPointF startPoint;
};
