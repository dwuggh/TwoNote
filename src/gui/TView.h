
#ifndef TWONOTE_TVIEW_H
#define TWONOTE_TVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLWidget>
#include <QtWidgets>
#include <QFileInfo>
#include <QFileDialog>
#include "TCanvas.h"

class TView : public QGraphicsView {

Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
    int height = 3000;
    int width  = 1920;
    QAction *drawModeAction;
    QAction *dragModeAction;
    TCanvas* currentBuffer;
    void loadFile(const QString& name);
    void saveBuffer(const QString& name = "");
    // switch buffer according to name.
    // since the name can be equal, a better practice is by uuid.
    void switchBuffer(const QString& name);
    // switch to a new buffer
    void switchBuffer();
    
public slots:
    void enableDragMode(bool checked);
    void enableDrawMode(bool checked);
    void save();
    // void load();

protected:
    void wheelEvent(QWheelEvent *) override;

private:
    QFile file;
    QString bufferName;
    // treat canvases as different buffer
    QList<TCanvas*> bufferList;
};

#endif
