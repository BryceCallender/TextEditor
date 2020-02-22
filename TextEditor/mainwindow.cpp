#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "texttabwidget.h"

#include <QTabBar>
#include <QDockWidget>
#include <QDebug>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuBar()->setNativeMenuBar(false);

    splitter = new QSplitter();
    splitter->addWidget(ui->tabWidget);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, new TextTabWidget(), "New Tab");

    connect(ui->tabWidget->tabBar(), &QTabBar::tabBarClicked, this, &MainWindow::setWindowToFileName);

//    QTabWidget *tabWidget = new QTabWidget();
//    tabWidget->addTab(new TextTabWidget(), "Tab");

//    splitter->addWidget(tabWidget);

//    splitter->setSizes(QList<int>({INT_MAX, 0}));

    setCentralWidget(splitter);

    track = 0;

    ui->tabWidget->setTabText(1, "+");

    ui->tabWidget->setCurrentIndex(0);

    QTabBar* tabBar = ui->tabWidget->tabBar();

    tabBar->tabButton(1, QTabBar::LeftSide)->deleteLater();
    tabBar->setTabButton(1, QTabBar::LeftSide, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
    ui->tabWidget->addTab(new TextTabWidget(), "New Tab");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }

    int tabIndex = ui->tabWidget->currentIndex();
    TextTabWidget* textTabWidget = (TextTabWidget*) ui->tabWidget->widget(tabIndex);

    textTabWidget->setTabsFileName(fileName);

    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    textTabWidget->setTextEditText(text);

    QFileInfo fileInfo(fileName);
    QString name(fileInfo.fileName());

    ui->tabWidget->setTabText(tabIndex, name); //calls setTabText(index of tab => int, name of file => QString);

    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    /*
    QString text = ui->textEdit->textCursor().selectedText();

    savedCopy[track] = text;
    if(track >= 2)
        track = 0;
    else
        track++;

    pasteMenu->addAction(text);
    QTextStream out(stdout);
    out << text << endl;*/

}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    /*
    QString text = ui->textEdit->textCursor().selectedText();

    savedCopy[track] = text;
    if(track >= 2)
        track = 0;
    else
        track++;
    ui->textEdit->textCursor().removeSelectedText();*/
}

void MainWindow::on_actionPaste_2_triggered()
{
    ui->textEdit->paste();
    //ui->textEdit->textCursor().insertText(savedCopy[0]);
    //pasteMenu->popup(ui->textEdit->textCursor().position());
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    //This will make sure to insert where the last tab is and they cannot move it or itll make a new tab
    if(index == ui->tabWidget->count() - 1)
    {
        ui->tabWidget->insertTab(index, new TextTabWidget(), "New tab");
    }
}

void MainWindow::on_actionView_Rendered_HTML_triggered()
{
    QWebEngineView *view = new QWebEngineView();

    view->load(QUrl::fromLocalFile(windowTitle()));
    view->show();
}

void MainWindow::on_actionSplit_Dock_Horizontally_triggered()
{
    splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
}

void MainWindow::on_actionSave_triggered()
{
    int tabIndex = ui->tabWidget->currentIndex();
    TextTabWidget* textTabWidget = (TextTabWidget*) ui->tabWidget->widget(tabIndex);

    QFile file(textTabWidget->getTabFileName());
    if(file.exists())
    {
        QTextStream out(&file);
        QString text = ui->textEdit->toPlainText();
        out << text;
        file.close();
    }
    else
    {
        on_actionSave_as_triggered();
    }

}

void MainWindow::setWindowToFileName(int index)
{
    TextTabWidget* textTabWidget = (TextTabWidget*) ui->tabWidget->widget(index);

    setWindowTitle(textTabWidget->getTabFileName());
}
