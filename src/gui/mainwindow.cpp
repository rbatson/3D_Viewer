#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("3D Viewer");
    glWidget = new GLWidget();
    ui->horizontalLayout->insertWidget(0, glWidget);
    connect(this, &MainWindow::readObjFile, glWidget, &GLWidget::readObjFile);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete glWidget;
}

void MainWindow::on_pushButton_openFile_clicked() {
  emit readObjFile();
}

