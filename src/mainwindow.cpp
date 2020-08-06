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
    scene = new TCanvas(view);
    view->setScene(scene);
    scene->setSceneRect(QRectF(-width / 2, height / 2, width, height));
    setCentralWidget(view);
    view->show();
}

void MainWindow::setupMenu() {
    menubar = new QMenuBar(this);
    menubar->setObjectName("menubar");
    menubar->setGeometry(QRect(0, 0, 720, 30));
    this->setMenuBar(menubar);

    QMenu* file = new QMenu(menubar);
    file->setObjectName("file");
    file->setTitle("file");

    file->addAction(scene->saveAction);
    file->addAction(scene->loadAction);

    menubar->addMenu(file);

    QMenu* preference = new QMenu(menubar);
    
}

void MainWindow::setupToolbar() {
    
    toolbar = new QToolBar;
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    toolbar->setGeometry(QRect(0, -30, 720, 30));
    this->addToolBar(toolbar);
    toolbar->addAction(view->drawModeAction);
    toolbar->addAction(view->dragModeAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}
