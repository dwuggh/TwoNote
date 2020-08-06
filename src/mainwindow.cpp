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
    TCanvas* canvas = view->currentBuffer;
    // view->setScene(canvas);
    canvas->setSceneRect(QRectF(-width / 2, height / 2, width, height));
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

    QAction* save = new QAction;
    save->setText("save");
    save->setChecked(false);
    save->setCheckable(false);
    QAction* load = new QAction;
    load->setText("load");
    load->setChecked(false);
    load->setCheckable(false);
    connect(save, &QAction::triggered, this->view, &TView::save);
    connect(load, &QAction::triggered, this, &MainWindow::load);
    // connect(load, &QAction::triggered, this->view, &TView::load);

    // file->addAction(canvas->saveAction);
    // file->addAction(canvas->loadAction);
    file->addAction(save);
    file->addAction(load);

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

void MainWindow::load() {
    QString name = QFileDialog::getOpenFileName
	(this, tr("Open .tnote"), "./", tr("twonote files (*.tnote)"));
    view->loadFileOrBuffer(name);
}

MainWindow::~MainWindow()
{
    delete ui;
}
