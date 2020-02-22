#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextEdit>
#include <QFile>
#include <QFileDialog>                  //to select files
#include <QTextStream>                  //reading text from files
#include <QMessageBox>                  //for user errors
#include <QtPrintSupport/QPrinter>      //print to printer
#include <QtPrintSupport/QPrintDialog>  //to select printer
#include <QClipboard>
#include <QMimeData>
#include <QTextStream>

#include <QSplitter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_2_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionSplit_Dock_Horizontally_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_tabBarClicked(int index);

    void on_actionView_Rendered_HTML_triggered();

    void on_actionSave_triggered();

    void setWindowToFileName(int index);

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    static const qint8 numCopies = 3;
    QString savedCopy[numCopies] = {"", "", ""};
    int track;
    QMenu *pasteMenu;

    QSplitter *splitter;
};
#endif // MAINWINDOW_H
