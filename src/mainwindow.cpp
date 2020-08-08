#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ui->setupUi(this);

    // this->showFullScreen();
    this->showMaximized();

    this->setupCanvas();
    this->setupToolbar();
    this->setupMenu();
}

void MainWindow::setupCanvas() {
    
    view = new TView(this);
    // canvas = new TCanvas(view);
    // TCanvas* canvas = view->currentBuffer;
    // view->setScene(canvas);
    // canvas->setSceneRect(QRectF(-width / 2, height / 2, width, height));
    setCentralWidget(view);
    view->show();
}

void MainWindow::setupMenu() {
    menubar = new QMenuBar(this);
    menubar->setObjectName("menubar");
    menubar->setGeometry(QRect(0, 0, 720, 30));
    this->setMenuBar(menubar);

    file = new QMenu(menubar);
    file->setObjectName("file");
    file->setTitle("file");

    saveBufferAction = new QAction;
    saveBufferAction->setText("save");
    saveBufferAction->setChecked(false);
    saveBufferAction->setCheckable(false);

    openFileAction = new QAction;
    openFileAction->setText("load");
    openFileAction->setChecked(false);
    openFileAction->setCheckable(false);

    saveBufferAsAction = new QAction;
    saveBufferAsAction->setText("save as");
    saveBufferAsAction->setChecked(false);
    saveBufferAsAction->setCheckable(false);

    createBufferAction = new QAction;
    createBufferAction->setText("new");
    createBufferAction->setChecked(false);
    createBufferAction->setCheckable(false);

    connect(saveBufferAction,   &QAction::triggered, this, &MainWindow::save);
    connect(openFileAction,     &QAction::triggered, this, &MainWindow::load);
    connect(saveBufferAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    connect(createBufferAction, &QAction::triggered, this, &MainWindow::create);
    // connect(load, &QAction::triggered, this->view, &TView::load);

    file->addAction(saveBufferAction);
    file->addAction(openFileAction);
    file->addAction(saveBufferAsAction);
    file->addAction(createBufferAction);

    menubar->addMenu(file);

    // QMenu* preference = new QMenu(menubar);
    
}

void MainWindow::setupToolbar() {
    
    toolbar = new QToolBar;
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    toolbar->setGeometry(QRect(0, -30, 720, 30));
    this->addToolBar(toolbar);
    toolbar->addAction(view->drawModeAction);
    toolbar->addAction(view->dragModeAction);
}

void MainWindow::load() {
    QString name = QFileDialog::getOpenFileName
	(this, tr("open file"), configs.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
    view->loadFile(name);
}

void MainWindow::create() {
    view->switchBuffer();
}

void MainWindow::save() {
    view->saveBuffer("");
}

void MainWindow::saveAs() {
    QString name = QFileDialog::getSaveFileName
	(this, tr("save file"), configs.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
    view->saveBuffer(name);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete view;
    delete file;
    delete menubar;
    delete toolbar;
    delete saveBufferAction;
    delete openFileAction;
    delete saveBufferAsAction;
    
}
