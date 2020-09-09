#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->showMaximized();
    ui->setupUi(this);

    // this->showFullScreen();

    this->setupCanvas();
    // this->setupToolbar();
    addToolBar(new TToolBar);
    this->setupMenu();
}

void MainWindow::setupCanvas() {

    view = new TView(this);
    setCentralWidget(view);
    view->show();
}

void MainWindow::setupMenu() {

    connect(ui->saveBufferAction, &QAction::triggered, this, &MainWindow::save);
    connect(ui->openFileAction, &QAction::triggered, this, &MainWindow::load);
    connect(ui->saveBufferAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->createBufferAction, &QAction::triggered, this, &MainWindow::create);
    // connect(load, &QAction::triggered, this->view, &TView::load);
}

void MainWindow::setupToolbar() {

    newPageAction = new QAction;
    newPageAction->setText("new page");
    newPageAction->setChecked(false);
    newPageAction->setCheckable(false);
    connect(newPageAction, &QAction::triggered, view, &TView::newPage);

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

    toolbar = new QToolBar;
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    toolbar->setGeometry(QRect(0, -30, 1920, 30));
    this->addToolBar(toolbar);

    editStateActionGroup = new QActionGroup(toolbar);
    editStateActionGroup->setExclusive(true);
    editStateActionGroup->addAction(drawModeAction);
    editStateActionGroup->addAction(dragModeAction);
    editStateActionGroup->addAction(typeModeAction);
    toolbar->addActions(editStateActionGroup->actions());

    toolbar->addSeparator();
    toolbar->addAction(newPageAction);
}

void MainWindow::load() {
    QString name = QFileDialog::getOpenFileName(
        this, tr("open file"), config.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
    view->loadFile(name);
}

void MainWindow::create() { view->switchBuffer(); }

void MainWindow::save() {
    if (view->currentBuffer->name.startsWith("/tmp")) {
        saveAs();
    } else {
        view->saveBuffer();
    }
}

inline void MainWindow::saveAs() {
    QString name = QFileDialog::getSaveFileName(
        this, tr("save file"), config.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
    view->saveBuffer(name);
}

MainWindow::~MainWindow() { delete ui; }
