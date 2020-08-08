#include "TPage.h"


TPage::TPage(int pageNumber, QSizeF pageSize) {
    this->pageNumber = pageNumber;
    this->pageSize = pageSize;
    this->lines = LineShapes();
}

QPointF TPage::scenePosToPagePos(QPointF& scenePoint) {
    return QPointF(
		   scenePoint.x(),
		   scenePoint.y() + pageNumber * pageSize.height()
		   );
}

QPointF TPage::pagePosToScenePos(QPointF& pagePoint) {
    return QPointF(
		   pagePoint.x(),
		   pagePoint.y() - pageNumber * pageSize.height()
		   );
}

LineShapes TPage::sceneLineShapes() {
    LineShapes sceneLines;
    for (LineShape line: lines) {
	LineShape sceneLine(line.width, line.color);
	for (QPointF point: line.points) {
	    sceneLine.append(pagePosToScenePos(point));
	}
    }
    return sceneLines;
}


QDebug operator<<(QDebug argument, const TPage &obj) {
    argument.nospace()
	<< "page: " << obj.pageNumber
	<< ", lines: " << obj.lines;
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