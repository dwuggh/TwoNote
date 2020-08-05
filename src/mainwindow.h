#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TCanvas.h"
#include <QOpenGLWidget>
#include "TView.h"
#include <QScrollBar>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QMenuBar* menubar;
    QToolBar* toolbar;
    QToolButton* drawModeButton;
    QToolButton* dragModeButton;
    
};
#endif // MAINWINDOW_H
