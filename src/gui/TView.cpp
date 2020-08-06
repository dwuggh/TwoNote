
#include "TView.h"


TView::TView(QWidget* parent) : QGraphicsView(parent) {

    this->setRenderHints(QPainter::Antialiasing |
			 QPainter::SmoothPixmapTransform |
			 QPainter::HighQualityAntialiasing);
    this->setViewport(new QOpenGLWidget);
    this->resize(res_width, res_height);
    this->centerOn(0, 0);

    this->changeCurrentBuffer(new TCanvas(this));
    // currentBuffer = new TCanvas(this);
    // this->setScene(currentBuffer);
    // file.setFileName(currentBuffer->bufferFileName);

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

void TView::save() {
    if (currentBuffer->bufferFileName != "") {
	currentBuffer->save();
    }
}

void TView::loadFileOrBuffer(const QString& name) {
    // https://stackoverflow.com/questions/10273816/how-to-check-whether-file-exists-in-qt-in-c/40203257
    bool file_exists = QFileInfo::exists(name) && QFileInfo(name).isFile();
    if (file_exists) {
	qDebug() << "TView: loading" << name;
	// currentBuffer = new TCanvas(name, this);
	this->changeCurrentBuffer(new TCanvas(name, this));
    } else {
	qDebug() << "TView: failed to load" << name;
	// exception
    }
}


TCanvas* TView::changeCurrentBuffer(TCanvas* newBuffer) {
    TCanvas* lastBuffer = this->currentBuffer;
    this->currentBuffer = newBuffer;
    this->bufferName = currentBuffer->bufferFileName;
    this->setScene(currentBuffer);
    // file.setFileName(currentBuffer->bufferFileName);
    return lastBuffer;
}
