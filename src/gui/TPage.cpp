#include "TPage.h"

TPage::TPage(int pageNumber, QSizeF pageSize) {
    this->pageNumber = pageNumber;
    this->pageSize = pageSize;
    this->lines = LineShapes();
    // this->pixmaps = QList<PixmapData>();
    // this->texts = QList<QGraphicsTextItem>();
}

QPointF TPage::mapToPage(const QPointF& scenePoint) {
    return QPointF(scenePoint.x(),
                   scenePoint.y() - pageNumber * pageSize.height());
}

QPointF TPage::mapToScene(const QPointF& pagePoint) {
    return QPointF(pagePoint.x(),
                   pagePoint.y() + pageNumber * pageSize.height());
}

const QRectF TPage::pageRect() {
    qreal width = pageSize.width();
    qreal height = pageSize.height();
    int margin = config.pageView.verticalMargin;
    return QRectF(-width / 2, -height / 2 + pageNumber * (height + margin),
                  width, height);
}

LineShapes TPage::sceneLineShapes() {
    LineShapes sceneLines;
    for (LineShape line : lines) {
        LineShape sceneLine(line.width, line.color);
        for (QPointF point : line.points) {
            sceneLine.append(mapToScene(point));
        }
        sceneLines.append(sceneLine);
    }
    return sceneLines;
}

void TPage::addLine(const LineShape& sceneLine) {
    LineShape line(sceneLine.width, sceneLine.color);
    for (QPointF p : sceneLine.points) {
        line.points.append(mapToPage(p));
    }
    // qDebug() << "adding line" << line;
    lines.append(line);
}

void TPage::addPixmap(const PixmapData& pixmapData) {
    PixmapData pixmap(pixmapData.pixmap, mapToPage(pixmapData.position),
                      pixmapData.scaleX, pixmapData.scaleY);
    pixmaps.append(pixmap);
}

QDebug operator<<(QDebug argument, const TPage& obj) {
    argument.nospace() << "page: " << obj.pageNumber << "lines: " << obj.lines
                       << "pixmaps:" << obj.pixmaps;
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, TPage& obj) {
    in >> obj.pageNumber >> obj.pageSize >> obj.lines >> obj.pixmaps;
    return in;
}
QDataStream& operator<<(QDataStream& out, const TPage& obj) {
    out << obj.pageNumber << obj.pageSize << obj.lines << obj.pixmaps;
    return out;
}

PixmapData::PixmapData(QPixmap pixmap, QPointF position, qreal scaleX,
                       qreal scaleY)
    : pixmap(pixmap), position(position), scaleX(scaleX), scaleY(scaleY){};

QDebug operator<<(QDebug argument, const PixmapData& obj) {
    argument.nospace() << "pixmap: " << obj.pixmap
                       << "position: " << obj.position
                       << "scale: " << obj.scaleX << " " << obj.scaleY;
    return argument.space();
}

QDataStream& operator>>(QDataStream& in, PixmapData& obj) {
    in >> obj.pixmap >> obj.position >> obj.scaleX >> obj.scaleY;
    return in;
}

QDataStream& operator<<(QDataStream& out, const PixmapData& obj) {
    out << obj.pixmap << obj.position << obj.scaleX << obj.scaleY;
    return out;
}
