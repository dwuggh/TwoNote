
#include "TView.h"


TView::TView(QWidget* parent) : QGraphicsView(parent) {

    this->setRenderHints(QPainter::Antialiasing |
			 QPainter::SmoothPixmapTransform |
			 QPainter::HighQualityAntialiasing);
    this->setViewport(new QOpenGLWidget);
    this->switchBuffer();



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
    if (currentBuffer->bufferName != "") {
	currentBuffer->save();
    }
}

void TView::loadFile(const QString& name) {
    // https://stackoverflow.com/questions/10273816/how-to-check-whether-file-exists-in-qt-in-c/40203257
    bool file_exists = QFileInfo::exists(name) && QFileInfo(name).isFile();
    if (file_exists) {
	qDebug() << "TView: loading" << name;
	// currentBuffer = new TCanvas(name, this);
	this->switchBuffer(name);
    } else {
	qDebug() << "TView: failed to load" << name;
	// exception
    }
}

void TView::saveBuffer(const QString &name) {
    if (name != "") {
	currentBuffer->saveAs(name);
    } else {
	currentBuffer->save();
    }
}

void TView::switchBuffer(const QString& name) {
    for (auto buffer: bufferList) {
	if (buffer->bufferName == name) {
	    currentBuffer = buffer;
	    bufferName = buffer->bufferName;
	    this->setScene(currentBuffer);
	    return;
        }
    }
    currentBuffer = new TCanvas(name, this);
    this->setScene(currentBuffer);
    bufferName = currentBuffer->bufferName;
    qDebug() << *currentBuffer;
    bufferList.append(currentBuffer);
}

void TView::switchBuffer() {
    // QString name = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    currentBuffer = new TCanvas(this);
    // currentBuffer->bufferName = name;
    currentBuffer->setSceneRect(QRectF(- width / 2, height / 2, width, height));
    this->setScene(currentBuffer);
    this->centerOn(0, 0);
    bufferName = currentBuffer->bufferName;
    qDebug() << *currentBuffer;
    bufferList.append(currentBuffer);
}

