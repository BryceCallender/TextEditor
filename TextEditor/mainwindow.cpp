#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuBar()->setNativeMenuBar(false);

    //ui->tabWidget->removeTab(1);
    ui->tabWidget->setDocumentMode(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
