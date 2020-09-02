#include "TView.h"

TView::TView(QWidget* parent) : QGraphicsView(parent) {

    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform |
                         QPainter::HighQualityAntialiasing);
    QOpenGLWidget* viewport = new QOpenGLWidget;
    QSurfaceFormat format;
    format.setSamples(4);
    viewport->setFormat(format);
    this->setViewport(viewport);
    this->switchBuffer();

    currentScale = 0.8;
    setTransform(QTransform::fromScale(currentScale, currentScale));

    setMouseTracking(true);
    // viewport->setMouseTracking(true);
}

void TView::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        int ds = event->angleDelta().y();
        zoom(ds);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void TView::zoom(int ds) {
    // scaling threshold, prevent flip
    if (currentScale < 0.1 && ds < 0) {
        return;
    }
    qreal factor = ds * 0.04 / 360;
    currentScale = currentScale + factor;
    // currentScale = factor;
    qDebug() << "from" << currentScale - factor
             << "scaling to:" << currentScale;
    setTransform(QTransform::fromScale(currentScale, currentScale));
}

void TView::enableDragMode(bool checked) {
    if (checked) {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        this->setInteractive(false);
	state.setView();
    }
}

void TView::enableDrawMode(bool checked) {
    if (checked) {
        this->setDragMode(QGraphicsView::NoDrag);
        this->setInteractive(true);
	state.setDraw();
    }
}

void TView::enableTypeMode(bool checked) {
    if (checked) {
        this->setDragMode(QGraphicsView::NoDrag);
        this->setInteractive(true);
	state.setType();
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

void TView::saveBuffer(const QString& name) {
    if (name == "") {
        currentBuffer->save();
    } else {
        currentBuffer->saveAs(name);
    }
}

void TView::switchBuffer(const QString& name) {
    for (auto buffer : bufferList) {
        if (buffer->name == name) {
            currentBuffer = buffer;
            bufferName = buffer->name;
            this->setScene(currentBuffer);
            return;
        }
    }
    currentBuffer = new TCanvas(name, this);
    this->setScene(currentBuffer);
    bufferName = currentBuffer->name;
    qDebug() << "switch to buffer:" << currentBuffer->name;
    bufferList.append(currentBuffer);
}

void TView::switchBuffer() {
    currentBuffer = new TCanvas(this);
    this->setScene(currentBuffer);
    this->centerOn(0, 0);
    bufferName = currentBuffer->name;
    qDebug() << "switch to buffer:" << currentBuffer->name;
    bufferList.append(currentBuffer);
}

void TView::newPage() { currentBuffer->newPage(); }

void TView::mouseMoveEvent(QMouseEvent* event) {
    int py = event->pos().y();
    int vy = size().height();
    int threshold = vy / 10;
    int dy = 20;
    qDebug() << py << vy;
    if (py > vy - threshold) {
	// qDebug() << "scroll down";
	QScrollBar* yPos = verticalScrollBar();
	yPos->setValue(yPos->value() + dy);
    } else if (py < threshold) {
	// qDebug() << "scroll up";
	QScrollBar* yPos = verticalScrollBar();
	yPos->setValue(yPos->value() - dy);
    }
    QGraphicsView::mouseMoveEvent(event);
}
