
#ifndef TWONOTE_TVIEW_H
#define TWONOTE_TVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>
#include <QDebug>
#include <QOpenGLWidget>

class TView : public QGraphicsView {

Q_OBJECT

public:
    explicit TView(QWidget *parent = nullptr);
    int res_height = 1920;
    int res_width = 1080;
    
protected:
 void wheelEvent(QWheelEvent *) override;
	
};

#endif
