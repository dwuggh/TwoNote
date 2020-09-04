#pragma once

#include "Configs.h"
#include "State.h"
#include "TLineItem.h"
#include "TPixmapItem.h"
#include <QDataStream>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QList>
#include <QMimeData>
#include <QRectF>
#include <QSharedPointer>
#include <QTextDocument>
#include <QTransform>
#include <QUndoCommand>
#include <QUndoStack>
#include <QUrl>

class AddItemCommand;

class TPage : public QGraphicsItem {
  public:
    explicit TPage(int pageNumber = 0, QPointF centralPoint = QPointF(0, 0),
                   QGraphicsItem* parent = nullptr);
    int pageNumber;
    QPointF centralPoint;
    QSizeF pageSize;

    QRectF boundingRect() const override;
    void setUndoStack(QSharedPointer<QUndoStack>& undoStack);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void addItem(QGraphicsItem* item);
    void removeItem(QGraphicsItem* item);

    // friend QDebug operator<<(QDebug argument, const TPage& obj);
    friend QDataStream& operator>>(QDataStream& in, TPage& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPage& obj);
    friend QDataStream& operator>>(QDataStream& in, TPage*& obj);
    friend QDataStream& operator<<(QDataStream& out, const TPage* obj);

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent*) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent*) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent*) override;
    void dropEvent(QGraphicsSceneDragDropEvent*) override;

  private:
    QList<TLineItem*> lineItems;
    QList<TPixmapItem*> pixmapItems;
    TLineItem* currentLineItem;
    QPointF currentPoint;
    QSharedPointer<QUndoStack> undoStack;
    // QTransform trans;
};

class AddItemCommand : public QUndoCommand {
  public:
    AddItemCommand(QGraphicsItem* item, TPage* page, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

  private:
    QGraphicsItem* item;
    TPage* page;

    friend class TPage;
};
