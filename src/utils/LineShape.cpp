//
// Created by dwuggh on 8/4/20.
//

#include "LineShape.h"


bool LineShape::operator==(const LineShape &other) const {
    return this->points == other.points &&
            this->color == other.color &&
            this->width == other.width;
}

LineShape::LineShape(qreal width, QColor color) {
    this->color = std::move(color);
    this->width = width;
    points = QList<QPointF>();
    this->setPen();
}

void LineShape::setPen() {

    pen = QPen();
    pen.setColor(color);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(width);
}

void LineShape::quadPaintLine(QPainter &painter) {
    QPainterPath linePaths;
    if (this->points.length() >= 1) {
        linePaths.moveTo(this->points[0]);
    }
    else return;

    for (int i = 1; i < this->points.length() - 2; i ++) {
        linePaths.quadTo(this->points[i], this->points[i+1]);
    }

    painter.setPen(pen);
    painter.drawPath(linePaths);
}

void LineShape::append(const QPointF &point) {
    points.append(point);
}

QDebug operator<<(QDebug argument, const LineShape &obj) {
    argument.nospace()
    << obj.width << ","
    << obj.color << ","
    << "[" << obj.points << "]";
    return argument.space();
}

QDataStream &operator>>(QDataStream &in, LineShape &obj) {
    // in >> obj.points >> obj.color >> obj.width;
    in >> obj.width >> obj.color >> obj.points;
    return in;
}

QDataStream &operator<<(QDataStream &out, const LineShape &obj) {
    out << obj.width << obj.color << obj.points;
    return out;
}
