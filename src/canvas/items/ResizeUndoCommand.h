#pragma once

#include <QDebug>
#include <QGraphicsItem>
#include <QList>
#include <QPointF>
#include <QTransform>
#include <QUndoCommand>

struct ResizeData {
    QGraphicsItem* item;
    QPointF oldPos;
    QPointF newPos;
    QTransform oldTransform;
    QTransform newTransform;
};

inline bool operator==(const ResizeData& lhs, const ResizeData& rhs);

class ResizeUndoCommand : public QUndoCommand {

  public:
    ResizeUndoCommand(QUndoCommand* parent = nullptr);
    void addItem(QGraphicsItem* item, const QPointF& oldPos, const QTransform& oldTransform,
                 bool check = false);
    void addItem(const ResizeData& data, bool check = false);

    void undo() override;
    void redo() override;

  private:
    QList<ResizeData> datas;
};
