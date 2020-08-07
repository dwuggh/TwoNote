#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TCanvas.h"
#include <QOpenGLWidget>
#include "TView.h"
#include <QScrollBar>
#include <QtWidgets>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    int width = 1920;
    int height = 1080;

private:
    Ui::MainWindow *ui;
    TView* view;
    QMenuBar* menubar;
    QToolBar* toolbar;
    QMenu* file;
    QAction* saveBufferAction;
    QAction* openFileAction;
    QAction* saveBufferAsAction;
    QAction* createBufferAction;

    void setupMenu();
    void setupToolbar();
    void setupCanvas();

private slots:
    void load();
    void save();
    void saveAs();
    void create();
    
};
#endif // MAINWINDOW_H
