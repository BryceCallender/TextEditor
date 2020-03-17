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
#include <QPrintPreviewDialog>
#include <QClipboard>
#include <QMimeData>
#include <QTextStream>
#include <QShortcut>

#include <QSplitter>
#include <QFont>
#include <QFontDialog>
#include <QtGui>
#include <QFontComboBox>
#include <QComboBox>

#include "texttabwidget.h"
#include "optionswindow.h"
#include "settingsmanager.h"
#include "customtabwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void clipboard_changed();

    void fileChanged();

    void showContextPasteMenu(const QPoint& pos);

    void on_actionPaste_triggered();

    void on_actionPaste_2_triggered();

    void on_actionPaste_3_triggered();

    void on_actionPaste_4_triggered();

    void on_actionPaste_5_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void showContextMenu(const QPoint&);

    void on_actionZoom_in_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionZoom_Standard_triggered();

    void on_actionSplit_Dock_Horizontally_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_tabBarClicked(int index);

    void on_actionView_Rendered_HTML_triggered();

    void on_actionSave_triggered();

    void setWindowToFileName(int index);

    void on_actionFormat_Text_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_actionUnderline_triggered();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontSizeComboBox_currentIndexChanged(int index);

    void on_fontSizeComboBox_activated(const QString &arg1);

    TextTabWidget* getCurrentTabWidget();

    void markTextTabAsClean(const QString& newPath);

    void on_actionOptions_triggered();

    void on_actionCenter_triggered();

    void on_actionAlign_Left_triggered();

    void on_actionAlign_Right_triggered();

    void printPreview(QPrinter *printer);

    void on_actionSave_2_triggered();

    void on_actionBullets_triggered();

    void on_actionNumbering_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    static const qint8 numCopies = 3;
    QString savedCopy[numCopies] = {"", "", ""};
    int track;
    QMenu *pasteMenu;
    int zoom;

    QFileSystemWatcher *fileWatcher;
    OptionsWindow* optionsWindow;

    QSplitter* splitter;

    SettingsManager* settings;

    QDockWidget* dock;

    QVector<CustomTabWidget*> tabWidgets;
};
#endif // MAINWINDOW_H
