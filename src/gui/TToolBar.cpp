#include "TToolBar.h"

TToolBar::TToolBar(QWidget* parent) : QToolBar(parent) {

    setObjectName(QString::fromUtf8("toolbar"));
    setGeometry(QRect(0, -30, 1920, 30));
    

    drawModeAction = new QAction;
    drawModeAction->setText(tr("draw"));
    drawModeAction->setCheckable(true);
    drawModeAction->setChecked(true);
    dragModeAction = new QAction;
    dragModeAction->setText(tr("drag"));
    dragModeAction->setCheckable(true);
    dragModeAction->setChecked(false);
    typeModeAction = new QAction;
    typeModeAction->setText(tr("type"));
    typeModeAction->setCheckable(true);
    typeModeAction->setChecked(false);

    connect(drawModeAction, &QAction::triggered, view, &TView::enableDrawMode);
    connect(dragModeAction, &QAction::triggered, view, &TView::enableDragMode);
    connect(typeModeAction, &QAction::triggered, view, &TView::enableTypeMode);

    editStateActionGroup = new QActionGroup(this);
    editStateActionGroup->setExclusive(true);
    editStateActionGroup->addAction(drawModeAction);
    editStateActionGroup->addAction(dragModeAction);
    editStateActionGroup->addAction(typeModeAction);
    addActions(editStateActionGroup->actions());

    addSeparator();

    newPageAction = new QAction;
    newPageAction->setText("new page");
    newPageAction->setChecked(false);
    newPageAction->setCheckable(false);
    connect(newPageAction, &QAction::triggered, view, &TView::newPage);
    addAction(newPageAction);

    addSeparator();

    setupPalette();
}

void TToolBar::setupPalette() {
    
    blackColorAction = new QAction;
    blackColorAction->setIcon(QPixmap("./assets/icon_black.png"));
    blackColorAction->setCheckable(true);
    blackColorAction->setChecked(false);
    redColorAction = new QAction;
    QIcon icon = QIcon("./assets/icon_red.png");
    redColorAction->setIcon(icon);
    redColorAction->setCheckable(true);
    redColorAction->setChecked(false);
    paletteActionGroup = new QActionGroup(this);
    paletteActionGroup->setExclusive(true);
    paletteActionGroup->addAction(blackColorAction);
    paletteActionGroup->addAction(redColorAction);
    addActions(paletteActionGroup->actions());
}
