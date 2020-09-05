#include "TRubberBandItem.h"

TRubberBandItem::TRubberBandItem(const QPointF& start, AreaType type, QGraphicsItem* parent) : TLineItem(start, parent) {
    areaType = type;
    startPoint = start;
    QPen pen = this->pen();
    pen.setStyle(Qt::PenStyle::DotLine);
    QAbstractGraphicsShapeItem::setPen(pen);
    setFlag(ItemIsSelectable, false);
    setFlag(ItemIsMovable, false);
}

void TRubberBandItem::addPoint(const QPointF& point, bool endOfLine) {
    switch (areaType) {
    case AreaType::Rect:
	path = QPainterPath();
	path.moveTo(startPoint);
	path.addRect(QRectF(startPoint, point));
	this->setPath(path);
	break;
    case AreaType::Any:
	TLineItem::addPoint(point, endOfLine);
	break;
    };
}

const QPainterPath& TRubberBandItem::selectionArea() {
    return path;
}
