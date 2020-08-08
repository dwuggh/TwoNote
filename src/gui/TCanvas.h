//
// Created by dwuggh on 8/3/20.
//

#ifndef TWONOTE_TCANVAS_H
#define TWONOTE_TCANVAS_H



#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QSizeF>
#include <QUuid>
#include "LineShape.h"
#include "TPage.h"
#include "Configs.h"


class TCanvas : public QGraphicsScene {

Q_OBJECT

public:
    explicit TCanvas(QWidget *parent = nullptr);
    TCanvas(const QString& name, QWidget *parent = nullptr);
    QString setName(const QString& name);
    void save();
    void saveAs(const QString& name);
    // buffer name can be different from file name
    QString bufferName;
    // assign a uuid for further identification
    QUuid uuid;

    friend QDebug operator<<(QDebug argument, const TCanvas &obj);
    friend QDataStream &operator>>(QDataStream &in, TCanvas &obj);
    friend QDataStream &operator<<(QDataStream &out, const TCanvas &obj);


public slots:
    // void save();
    // void load();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
    void wheelEvent(QGraphicsSceneWheelEvent *) override;
    // void paintEvent(QPaintEvent *) override;
private:
    QFile file;
    LineShapes lineShapes;
    LineShape currentLine;
    bool isDrawing = false;
    QPointF currentPoint;
    QPointF lastPoint;
    QPainterPath currentPath;
    QGraphicsPathItem* currentPathItem;
    QPen pen;
    QList<TPage> pages;
    int currentPageNumber;
    int pageCounts;
    QSizeF pageSize;
    void paintLines();
    void paintPages();
    int choosedPage(QPointF& scenePoint);
    
};

typedef QList<TCanvas> TCanvasList;

#endif //TWONOTE_TCANVAS_H
