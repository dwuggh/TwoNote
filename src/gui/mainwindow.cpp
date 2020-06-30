#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setCentralWidget(new DrawBoardWidget(this));
    // setCentralWidget(new ScratchWidget(this));
    ScratchView* view = new ScratchView(this);
    // ScratchScene* scene = new ScratchScene(view);
    // view->setScene(scene);
    // scene->addRect(QRectF(1, 1, 0, 50));

    setCentralWidget(view);
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

