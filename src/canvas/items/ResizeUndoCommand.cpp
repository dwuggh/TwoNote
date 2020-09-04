#include "ResizeUndoCommand.h"

inline bool operator==(const ResizeData& lhs, const ResizeData& rhs) {
    return lhs.item == rhs.item && lhs.oldPos == rhs.oldPos && lhs.newPos == rhs.newPos &&
           lhs.oldTransform == rhs.oldTransform && lhs.newTransform == rhs.newTransform;
}

ResizeUndoCommand::ResizeUndoCommand(QUndoCommand* parent) : QUndoCommand(parent) {}

void ResizeUndoCommand::addItem(const ResizeData& data, bool check) {
    if (check && datas.contains(data)) {
        qDebug() << "ResizeUndoCommand::already has data";
        return;
    }
    datas.append(data);
}

void ResizeUndoCommand::addItem(QGraphicsItem* item, const QPointF& oldPos,
                                       const QTransform& oldTransform, bool check) {
    ResizeData data = ResizeData{.item         = item,
                                 .oldPos       = oldPos,
                                 .newPos       = item->pos(),
                                 .oldTransform = oldTransform,
                                 .newTransform = item->transform()};
    addItem(data, check);
}

void ResizeUndoCommand::undo() {
    for (auto data : datas) {
        auto* item = data.item;
        item->setPos(data.oldPos);
        item->setTransform(data.oldTransform);
    }
}

void ResizeUndoCommand::redo() {
    for (auto data : datas) {
        auto* item = data.item;
        item->setPos(data.newPos);
        item->setTransform(data.newTransform);
    }
}
