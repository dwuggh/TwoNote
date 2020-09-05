#pragma once

#include <QDataStream>
#include <QDebug>
#include <QGraphicsItem>
#include <QList>
#include <QPointF>
#include <QTransform>
#include <QUndoCommand>

struct TransformData {
    QGraphicsItem* item;
    QPointF oldPos;
    QPointF newPos;
    QTransform oldTransform;
    QTransform newTransform;
};

inline bool operator==(const TransformData& lhs, const TransformData& rhs);
QDebug operator<<(QDebug argument, const TransformData& obj);
// QDataStream& operator<<(QDataStream& out, const ResizeData& obj);
// QDataStream& operator>>(QDataStream& in, ResizeData& obj);

class TransformUndoCommand : public QUndoCommand {

  public:
    TransformUndoCommand(QUndoCommand* parent = nullptr);
    void addItem(QGraphicsItem* item, const QPointF& oldPos, const QTransform& oldTransform,
                 bool check = false);
    void addItem(const TransformData& data, bool check = false);
    void registerItem(QGraphicsItem* item);
    void update();

    void undo() override;
    void redo() override;

  private:
    QList<TransformData> datas;
};
