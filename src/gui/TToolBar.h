#pragma once

#include "TView.h"
#include "State.h"
#include <QToolBar>

class TToolBar : public QToolBar {
    Q_OBJECT

 public:
    explicit TToolBar(QWidget* parent = nullptr);

 private:
   TView* view;

    QActionGroup* editStateActionGroup;
    QAction* newPageAction;
    QAction* drawModeAction;
    QAction* dragModeAction;
    QAction* typeModeAction;

    QActionGroup* paletteActionGroup;
    QAction* blackColorAction;
    QAction* redColorAction;

    void setupPalette();
};
