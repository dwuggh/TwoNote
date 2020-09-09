#pragma once

#include <QPen>

enum EditState { draw, type, view };

class State {
  public:
    EditState editState;
    State();
    void setDraw() { editState = EditState::draw; }
    void setType() { editState = EditState::type; }
    void setView() { editState = EditState::view; }

    QPen pen;
};

extern State state;
