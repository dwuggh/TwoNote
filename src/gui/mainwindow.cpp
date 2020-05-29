#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawboardwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new DrawBoardWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

