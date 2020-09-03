#pragma once

#include "Configs.h"
#include "State.h"
#include "TPage.h"
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMimeData>
#include <QPainterPath>
#include <QSharedPointer>
#include <QSizeF>
#include <QTextDocument>
#include <QUndoCommand>
#include <QUndoStack>
#include <QUuid>
#include <QWidget>

// enum EditState { draw, type, view };

class TCanvas : public QGraphicsScene {

    Q_OBJECT

  public:
    explicit TCanvas(QWidget* parent = nullptr);
    TCanvas(const QString& name, QWidget* parent = nullptr);
    // buffer name can be different from file name
    QString name;
    // assign a uuid for further identification
    QUuid uuid;

    QString setName(const QString& name);
    void save();
    void saveAs(const QString& name);
    int newPage();
    // void undoRegister(TPage);

    friend QDebug operator<<(QDebug argument, const TCanvas& obj);
    friend QDataStream& operator>>(QDataStream& in, TCanvas& obj);
    friend QDataStream& operator<<(QDataStream& out, const TCanvas& obj);

  protected:
    void keyPressEvent(QKeyEvent*) override;
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent*) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent*) override;
    void dropEvent(QGraphicsSceneDragDropEvent*) override;
    void wheelEvent(QGraphicsSceneWheelEvent*) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    // void paintEvent(QPaintEvent *) override;
  private:
    QFile file;
    QPointF currentPoint;
    QPointF lastPoint;
    QGraphicsTextItem* item;
    QPen pen;
    QList<TPage*> pages;
    int currentPageNumber;
    int pageCounts;
    QSizeF pageSize;
    QSharedPointer<QUndoStack> undoStack;
    bool contains(const QPointF& point) const;
    int choosedPage(QPointF& scenePoint);
    void updateSceneRect();
};
