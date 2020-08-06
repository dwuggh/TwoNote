
#ifndef TWONOTE_TVIEW_H
#define TWONOTE_TVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLWidget>
#include <QtWidgets>
#include <QFileInfo>
#include "TCanvas.h"

class TView : public QGraphicsView {

Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
    int res_height = 1920;
    int res_width = 1080;
    QAction *drawModeAction;
    QAction *dragModeAction;
    TCanvas* currentBuffer;
    
public slots:
    void enableDragMode(bool checked);
    void enableDrawMode(bool checked);
    void save();
    void load();

protected:
    void wheelEvent(QWheelEvent *) override;

private:
    QFile file;
    QString bufferName;
    // treat canvases as different buffer
    TCanvasList bufferList;
    TCanvas* changeCurrentBuffer(TCanvas* newBuffer);
};

#endif
