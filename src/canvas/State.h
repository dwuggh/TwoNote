#pragma once

enum EditState { draw, type, view };

class State {
  public:
    EditState editState;
    State() { editState = EditState::draw; }
    void setDraw() { editState = EditState::draw; }
    void setType() { editState = EditState::type; }
    void setView() { editState = EditState::view; }
};

extern State state;
