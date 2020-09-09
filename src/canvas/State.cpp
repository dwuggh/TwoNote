#include "State.h"

State state = State();

State::State() {
    editState = EditState::draw;
    pen = QPen();
    pen.setColor(Qt::black);
    pen.setWidthF(3.0);
    pen.setJoinStyle(Qt::RoundJoin);
}
