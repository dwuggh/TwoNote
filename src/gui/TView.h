
#ifndef TWONOTE_TVIEW_H
#define TWONOTE_TVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLWidget>
#include <QtWidgets>

class TView : public QGraphicsView {

Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
    int res_height = 1920;
    int res_width = 1080;
    QToolButton *drawModeButton;
    QToolButton *dragModeButton;
    
protected:
    void wheelEvent(QWheelEvent *) override;

private slots:
    void enableDragMode(bool checked);
    void enableDrawMode(bool checked);
	
    
	
};

#endif
