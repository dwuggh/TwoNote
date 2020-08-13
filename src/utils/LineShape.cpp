#include "LineShape.h"

LineShape::LineShape(qreal width, QColor color) {
    this->color = std::move(color);
    this->width = width;
    points = QList<QPointF>();
    setPen();
}

LineShape::LineShape(const LineShape& other) {
    this->color = other.color;
    this->width = other.width;
    this->points = QList<QPointF>(other.points);
    this->setPen();
}

bool LineShape::operator==(const LineShape& other) const {
    return this->points == other.points && this->color == other.color &&
           this->width == other.width;
}

void LineShape::setPen() {

    pen.setColor(color);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidthF(width);
}

void LineShape::append(const QPointF& point) { points.append(point); }

QDebug operator<<(QDebug argument, const LineShape& obj) {
    argument.nospace() << obj.width << "," << obj.color << ","
                       << "[" << obj.points << "]";
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, LineShape& obj) {
    // in >> obj.points >> obj.color >> obj.width;
    in >> obj.width >> obj.color >> obj.points;
    obj.setPen();
    return in;
}

QDataStream& operator<<(QDataStream& out, const LineShape& obj) {
    out << obj.width << obj.color << obj.points;
    return out;
}
