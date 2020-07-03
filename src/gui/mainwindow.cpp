#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setCentralWidget(new DrawBoardWidget(this));
    // setCentralWidget(new ScratchWidget(this));
    QGraphicsView* view = new QGraphicsView(this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setRenderHint(QPainter::HighQualityAntialiasing);
    ScratchScene* scene = new ScratchScene(view);
    // scene->addRect(QRectF(0, 0, 100, 100));
    view->setScene(scene);
    // scene->addRect(QRectF(1, 1, 0, 50));

    setCentralWidget(view);
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

