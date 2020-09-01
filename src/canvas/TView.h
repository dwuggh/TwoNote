#pragma once

#include "Configs.h"
#include "TCanvas.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsView>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QWheelEvent>

// extern Configs config;

class TView : public QGraphicsView {

    Q_OBJECT

  public:
    explicit TView(QWidget* parent = nullptr);
    TCanvas* currentBuffer;
    void loadFile(const QString& name);
    void saveBuffer(const QString& name = "");
    // switch buffer according to name.
    // since the name can be equal, a better practice is by uuid.
    void switchBuffer(const QString& name);
    // switch to a new buffer
    void switchBuffer();
    void zoom(int ds);

  public slots:
    void enableDragMode(bool checked);
    void enableDrawMode(bool checked);
    void enableTypeMode(bool checked);
    void newPage();
    // void load();

  protected:
    void wheelEvent(QWheelEvent*) override;

  private:
    QFile file;
    QString bufferName;
    qreal currentScale = 0.8;
    // treat canvases as different buffer
    QList<TCanvas*> bufferList;
};
