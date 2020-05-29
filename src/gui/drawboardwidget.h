#ifndef DRAWBOARDWIDGET_H_
#define DRAWBOARDWIDGET_H_

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QPainter>
#include <qcolor.h>
#include <qglobal.h>
#include <QMouseEvent>
// #include <qpainter.h>

QT_BEGIN_NAMESPACE
namespace DrawboardWidget {
    class DrawboardWidget;
}

class DrawBoardWidget : public QWidget {

    Q_OBJECT

public:
    DrawBoardWidget(QWidget *parent = nullptr);

public slots:
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void paint(QImage &image);
    void drawLine(const QPoint &p1, const QPoint &p2);

signals:

private:

    QImage image;
    QRgb bgColor;
    QPoint lastPoint;
    QPoint currentPoint;
    bool isDrawing;
    QColor penColor;
    qreal penWidth;


};


#endif
