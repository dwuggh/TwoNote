
#ifndef TWONOTE_TVIEW_H
#define TWONOTE_TVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLWidget>
#include <QtWidgets>
#include <QFileInfo>
#include <QFileDialog>
#include "Configs.h"
#include "TCanvas.h"

// extern Configs config;

class TView : public QGraphicsView {

Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
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
    void zoom(int ds);

    
public slots:
    void enableDragMode(bool checked);
    void enableDrawMode(bool checked);
    void save();
    void newPage();
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
