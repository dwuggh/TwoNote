
#include "TView.h"


TView::TView(QWidget* parent) : QGraphicsView(parent) {

    this->setRenderHints(QPainter::Antialiasing |
			 QPainter::SmoothPixmapTransform |
			 QPainter::HighQualityAntialiasing);
    this->setViewport(new QOpenGLWidget);
    this->resize(res_width, res_height);
    this->centerOn(0, 0);

    drawModeButton = new QToolButton;
    drawModeButton->setText(tr("draw"));
    drawModeButton->setCheckable(true);
    drawModeButton->setChecked(true);
    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("drag"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);
    
    connect(drawModeButton, &QAbstractButton::toggled, this, &TView::enableDrawMode);
    connect(dragModeButton, &QAbstractButton::toggled, this, &TView::enableDragMode);
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
	dragModeButton->setChecked(checked);
	drawModeButton->setChecked(!checked);
	this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setInteractive(false);
	// dragModeButton->setCheckable(false);
    } else {
	// dragModeButton->setChecked(true);
    }
}

void TView::enableDrawMode(bool checked) {
    if (checked) {
	dragModeButton->setChecked(!checked);
	drawModeButton->setChecked(checked);
	this->setDragMode(QGraphicsView::NoDrag);
	this->setInteractive(true);
	// drawModeButton->setCheckable(false);
    } else {
	// drawModeButton->setChecked(true);
    }
}
