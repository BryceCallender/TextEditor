#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuBar()->setNativeMenuBar(false);
    track = 0;
    QComboBox* fontFamily = ui->fontComboBox;
    QComboBox* fontSize = ui->fontSizeComboBox;
    ui->toolBar->addWidget(fontSize);
    ui->toolBar->setStyleSheet("QToolBar{spacing:3px;}");
    ui->toolBar->addWidget(fontFamily);
    //ui->tabWidget->removeTab(1);
    //ui->tabWidget->setDocumentMode(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
    //TODO: open as new tab?
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
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);

    QFileInfo fileInfo(fileName);
    QString name(fileInfo.fileName());
    ui->tabWidget->setTabText(0, name);     //setTabText(index of tab => int, name of file => QString);
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


void MainWindow::on_actionFormat_Text_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);

    if(ok)
    {
        ui->textEdit->setFont(font);
    }
    else
        return;
}

void MainWindow::on_actionBold_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;

    if(cursor.charFormat().fontWeight() == QFont::Bold)
    {
        format.setFontWeight(QFont::Normal);
    }
    else
        format.setFontWeight(QFont::Bold);

    cursor.mergeCharFormat(format);//do the text as Bold
}

void MainWindow::on_actionItalic_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;

    if(cursor.charFormat().fontItalic())
    {
        format.setFontItalic(false);
    }
    else
        format.setFontItalic(true);

    cursor.mergeCharFormat(format);//do the text as italic
}

void MainWindow::on_actionUnderline_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;

    if(cursor.charFormat().fontUnderline())
    {
        format.setFontUnderline(false);
    }
    else
        format.setFontUnderline(true);

    cursor.mergeCharFormat(format);//do the text as underline
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    //QFont font = ui->fontComboBox->currentFont();

    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;
    format.setFont(f);

    cursor.mergeCharFormat(format);//Change font-family
}

void MainWindow::on_fontSizeComboBox_activated(const QString &arg1)
{
    QString size = ui->fontSizeComboBox->currentText();
    QFont font;
    font.setPointSize(size.toInt());
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format;
    format.setFont(font);

    cursor.mergeCharFormat(format);//change font size
}
