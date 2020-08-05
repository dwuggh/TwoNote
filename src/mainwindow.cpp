#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // ui->setupUi(this);
    // setCentralWidget(new DrawBoardWidget(this));
    // setCentralWidget(new ScratchWidget(this));
    // setCentralWidget(new TCanvas(this));

    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 720, 30));
    this->setMenuBar(menubar);

    toolbar = new QToolBar;
    toolbar->setObjectName(QString::fromUtf8("toolbar"));
    toolbar->setGeometry(QRect(0, 0, 720, 30));
    this->addToolBar(toolbar);
    
    // drawModeButton = new QToolButton;
    // drawModeButton->setText(tr("draw"));
    // drawModeButton->setCheckable(true);
    // drawModeButton->setChecked(true);
    // dragModeButton = new QToolButton;
    // dragModeButton->setText(tr("drag"));
    // dragModeButton->setCheckable(true);
    // dragModeButton->setChecked(false);
    
    
    // this->setStatusBar(new QStatusBar(this));

    this->resize(1920, 1080);
    TView *view = new TView(this);
    TCanvas *scene = new TCanvas(view);
    view->setScene(scene);
    setCentralWidget(view);
    scene->setSceneRect(QRectF(-1920 / 2, 1080 / 2, 1920, 1080));
    view->show();
    toolbar->addWidget(view->drawModeButton);
    toolbar->addWidget(view->dragModeButton);
    toolbar->addWidget(scene->saveButton);
    toolbar->addWidget(scene->loadButton);
    // connect(view->drawModeButton, &QAbstractButton::toggled, this, &TView::enableDrawMode);
    // connect(view->dragModeButton, &QAbstractButton::toggled, this, &TView::enableDragMode);
    //    ScratchScene* scene = new ScratchScene(view);
    //    // scene->addRect(QRectF(0, 0, 100, 100));
    //    view->setScene(scene);
    // scene->addRect(QRectF(1, 1, 0, 50));

}

MainWindow::~MainWindow()
{
    delete ui;
}

