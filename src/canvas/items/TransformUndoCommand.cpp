#include "TransformUndoCommand.h"

inline bool operator==(const TransformData& lhs, const TransformData& rhs) {
    return lhs.item == rhs.item && lhs.oldPos == rhs.oldPos && lhs.newPos == rhs.newPos &&
           lhs.oldTransform == rhs.oldTransform && lhs.newTransform == rhs.newTransform;
}

QDebug operator<<(QDebug argument, const TransformData& obj) {
    argument.nospace() << "Transform Data:\n"
		       << "item: " << obj.item
		       << "\n old pos: " << obj.oldPos
		       << "\t new pos: " << obj.newPos
                       << "\n new transform: " << obj.oldTransform
		       << "\t new transform: " << obj.newTransform;
    return argument.space();
}

TransformUndoCommand::TransformUndoCommand(QUndoCommand* parent) : QUndoCommand(parent) {}

void TransformUndoCommand::addItem(const TransformData& data, bool check) {
    if (check && datas.contains(data)) {
        qDebug() << "TransformUndoCommand::already has data";
        return;
    }
    datas.append(data);
}

void TransformUndoCommand::addItem(QGraphicsItem* item, const QPointF& oldPos,
                                   const QTransform& oldTransform, bool check) {
    TransformData data = TransformData{.item         = item,
                                       .oldPos       = oldPos,
                                       .newPos       = item->pos(),
                                       .oldTransform = oldTransform,
                                       .newTransform = item->transform()};
    addItem(data, check);
}

void TransformUndoCommand::registerItem(QGraphicsItem* item) {
    for (auto& d : datas) {
        if (d.item == item) {
            qDebug() << "already registered: " << item;
            return;
        }
    }
    datas.append(TransformData{
        .item         = item,
        .oldPos       = item->pos(),
        .oldTransform = item->transform(),
    });
}

void TransformUndoCommand::update() {
    for (auto& data : datas) {
        auto item         = data.item;
        data.newPos       = item->pos();
        data.newTransform = item->transform();
	// qDebug() << data;
        // qDebug() << item->pos() << item->transform();
    }
    qDebug() << datas;
}

void TransformUndoCommand::undo() {
    for (auto& data : datas) {
        auto* item = data.item;
        item->setPos(data.oldPos);
	qDebug() << "undo: \n" << data;
        item->setTransform(data.oldTransform);
    }
}

void TransformUndoCommand::redo() {
    for (auto& data : datas) {
        auto* item = data.item;
        item->setPos(data.newPos);
	qDebug() << "redo: \n" << data;
        item->setTransform(data.newTransform);
    }
}
