#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("3D Viewer");
    glWidget = new GLWidget();
    tmr = new QTimer();
    ui->horizontalLayout->insertWidget(0, glWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glWidget;
}
