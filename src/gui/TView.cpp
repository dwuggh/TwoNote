
#include "TView.h"


TView::TView(QWidget* parent) : QGraphicsView(parent) {

   this->setRenderHints(
			QPainter::Antialiasing
		      | QPainter::SmoothPixmapTransform
		      | QPainter::HighQualityAntialiasing
			);
   this->setViewport(new QOpenGLWidget);
   this->resize(res_width, res_height);
   this->centerOn(0, 0);

}

void TView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
	if (event->angleDelta().y() > 0) {
	    // zoom
	}
    } else {
	QGraphicsView::wheelEvent(event);
    }
}
