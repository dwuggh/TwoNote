#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->showMaximized();
    ui->setupUi(this);

    // this->showFullScreen();

    this->setupCanvas();
    this->setupToolbar();
    this->setupMenu();
    qDebug() << ui->menubar->size();
    qDebug() << view->size();
    qDebug() << this->size();
}

void MainWindow::setupCanvas() {
    
    view = new TView(this);
    view->setGeometry(QRect());
    setCentralWidget(view);
    view->show();
}

void MainWindow::setupMenu() {

    connect(ui->saveBufferAction,   &QAction::triggered, this, &MainWindow::save);
    connect(ui->openFileAction,     &QAction::triggered, this, &MainWindow::load);
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

    toolbar = new QToolBar;
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    toolbar->setGeometry(QRect(0, -30, 720, 30));
    this->addToolBar(toolbar);
    toolbar->addAction(view->drawModeAction);
    toolbar->addAction(view->dragModeAction);
    toolbar->addAction(newPageAction);
}

void MainWindow::load() {
    QString name = QFileDialog::getOpenFileName
	(this, tr("open file"), config.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
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
	(this, tr("save file"), config.baseDir.absolutePath(), tr("twonote files (*.tnote)"));
    view->saveBuffer(name);
}


MainWindow::~MainWindow()
{
    delete ui;
    
}
