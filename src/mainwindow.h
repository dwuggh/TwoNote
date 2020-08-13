#pragma once

#include "Configs.h"
#include "TCanvas.h"
#include "TView.h"
#include <QActionGroup>
#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QScrollBar>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    TView* view;
    QToolBar* toolbar;
    QActionGroup* editStateActionGroup;
    QAction* newPageAction;
    QAction* drawModeAction;
    QAction* dragModeAction;
    QAction* typeModeAction;

    void setupMenu();
    void setupToolbar();
    void setupCanvas();

  private slots:
    void load();
    void save();
    void saveAs();
    void create();
};
