#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QtWidgets>
#include <QFileDialog>
#include <QActionGroup>
#include "TCanvas.h"
#include "TView.h"
#include "Configs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TView* view;
    QToolBar* toolbar;
    QActionGroup* editStateActionGroup;
    QAction* newPageAction;
    QAction *drawModeAction;
    QAction *dragModeAction;
    QAction *typeModeAction;

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
