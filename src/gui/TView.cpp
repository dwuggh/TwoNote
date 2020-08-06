
#include "TView.h"


TView::TView(QWidget* parent) : QGraphicsView(parent) {

    this->setRenderHints(QPainter::Antialiasing |
			 QPainter::SmoothPixmapTransform |
			 QPainter::HighQualityAntialiasing);
    this->setViewport(new QOpenGLWidget);
    this->resize(res_width, res_height);
    this->centerOn(0, 0);

    drawModeAction = new QAction;
    drawModeAction->setText(tr("draw"));
    drawModeAction->setCheckable(true);
    drawModeAction->setChecked(true);
    dragModeAction = new QAction;
    dragModeAction->setText(tr("drag"));
    dragModeAction->setCheckable(true);
    dragModeAction->setChecked(false);
    
    connect(drawModeAction, &QAction::triggered, this, &TView::enableDrawMode);
    connect(dragModeAction, &QAction::triggered, this, &TView::enableDragMode);
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

void TView::enableDragMode(bool checked) {
    if (checked) {
	dragModeAction->setChecked(checked);
	drawModeAction->setChecked(!checked);
	this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setInteractive(false);
	// dragModeButton->setCheckable(false);
    } else {
	dragModeAction->setChecked(true);
    }
}

void TView::enableDrawMode(bool checked) {
    if (checked) {
	dragModeAction->setChecked(!checked);
	drawModeAction->setChecked(checked);
	this->setDragMode(QGraphicsView::NoDrag);
	this->setInteractive(true);
	// drawModeButton->setCheckable(false);
    } else {
	drawModeAction->setChecked(true);
    }
}
