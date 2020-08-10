#include "TPage.h"


TPage::TPage(int pageNumber, QSizeF pageSize) {
    this->pageNumber = pageNumber;
    this->pageSize = pageSize;
    this->lines = LineShapes();
}

QPointF TPage::mapToPage(QPointF &scenePoint) {
  return QPointF(scenePoint.x(),
                 scenePoint.y() - pageNumber * pageSize.height());
}

QPointF TPage::mapToScene(QPointF &pagePoint) {
  return QPointF(pagePoint.x(), pagePoint.y() + pageNumber * pageSize.height());
}

const QRectF TPage::pageRect() {
    qreal width = pageSize.width();
    qreal height = pageSize.height();
    int margin = config.pageView.verticalMargin;
    return QRectF(
		  -width / 2,
		  -height / 2 + pageNumber * ( height + margin ),
		  width,
		  height
		  );
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

void TPage::addLine(const LineShape &sceneLine) {
  LineShape line(sceneLine.width, sceneLine.color);
  for (QPointF p : sceneLine.points) {
    line.points.append(mapToPage(p));
  }
  // qDebug() << "adding line" << line;
  lines.append(line);
}

QDebug operator<<(QDebug argument, const TPage &obj) {
  argument.nospace() << "page: " << obj.pageNumber << ", lines: " << obj.lines;
  return argument.space();
}

QDataStream &operator>>(QDataStream &in, TPage &obj) {
  in >> obj.pageNumber >> obj.pageSize >> obj.lines;
  return in;
}
QDataStream &operator<<(QDataStream &out, const TPage &obj) {
  out << obj.pageNumber << obj.pageSize << obj.lines;
  return out;
}
