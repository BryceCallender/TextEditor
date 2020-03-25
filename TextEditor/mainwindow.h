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

/*! \class MainWindow
    \brief The main window and view for the editor

    The main window which holds all the editor tools and formatting options. I holds all the tabs and allows for the special functionality.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! Constructor
    /*!
        Initializes the UI and other functionality of the window.
     */
    MainWindow(QWidget *parent = nullptr);

    //! Destructor
    /*!
        Detroys all the data and UI freeing the memory used.
     */
    ~MainWindow();

    TextTabWidget* getCurrentTabWidget();
    void setWindowToFileName(int index);
    void markTextTabAsClean(const QString& newPath);
    void printPreview(QPrinter *printer);

    //! When Window is closed completely
    /*!
        \param event The event where teh user pressed the X button to close the window.
     */
    void closeEvent(QCloseEvent *event);

    //! Command for when to remove a tab from a specified widget index
    /*!
        \param widgetIndex The widget index in the vector of tab widgets
        \param tabIndex the tab index from the parent widget index
     */
    void removeTabFromWidget(int widgetIndex, int tabIndex);

    //! Gets the tab widgets from the main window
    /*!
      Getter for other classes to grab the tab widgets associated with this window
     */
    QVector<CustomTabWidget*> getTabWidgets();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

    //! When the copy keyword is pressed in the toolbar
    /*!
        Executes the new copy functionality to store multiple copies.
     */
    void on_actionCopy_triggered();

    //! When the cut keyword is pressed in the toolbar
    /*!
        Executes the new cut functionality to store multiple copies.
     */
    void on_actionCut_triggered();

    //! Whenever the clipboard is changed using the shortcuts
    /*!
        Makes sure that if shortcuts are used, the new thing copied gets put into the multiple copy storage.
     */
    void clipboard_changed();

    //! Whenever something in the current file is changed
    /*!
        Adds an asterisk into the tab name so that the user knows that the file is changed.
     */
    void fileChanged();

    //! Builds a context menu for pasting
    /*!
        Executes a context menu that shows the current things being stored in the mulit past storage and allows them to be clicked for pasting.
     */
    void showContextPasteMenu(const QPoint& pos);

    //! When paste in toolbar is pressed
    /*!
        Sets the position of where to build the context menu for pasting and executes it.
     */
    void on_actionPaste_triggered();

    //! When topmost section of context menu is pressed
    /*!
        Pastes in the first section of the paste storage and removes the context menu.
     */
    void on_actionPaste_2_triggered();

    //! When middle section of context menu is pressed
    /*!
        Pastes in the second section of the paste storage and removes the context menu.
     */
    void on_actionPaste_3_triggered();

    //! When bottommost section of context menu is pressed
    /*!
        Pastes in the last section of the paste storage and removes the context menu.
     */
    void on_actionPaste_4_triggered();

    void on_actionPaste_5_triggered();

    //! When undo button is pressed in the toolbar
    /*!
        Undoes the previous action in the main window
     */
    void on_actionUndo_triggered();

    //! When redo button is pressed in the toolbar
    /*!
        Redoes the previous undo action in the main window
     */
    void on_actionRedo_triggered();

    //! When the user right clicks in the main window.
    /*!
        Shows a custom context menu at the click location that can undo, redo, cut, copy, paste, and zoom.
     */
    void showContextMenu(const QPoint&);

    //! When zoom in button is pressed in the toolbar
    /*!
        Zooms in the text on the screen by increasing the font size.
     */
    void on_actionZoom_in_triggered();

    //! When zoom out button is pressed in the toolbar
    /*!
        Zooms out the text on the screen by decreasing the font size.
     */
    void on_actionZoom_Out_triggered();

    //! When zoom standard button is pressed in the toolbar
    /*!
        Resets the zoom to the previous font size that the user specified.
     */
    void on_actionZoom_Standard_triggered();

    void on_actionSplit_Dock_Horizontally_triggered();

    //! When user tries to close a tab
    /*!
        Checks to see if the user has changed the file in the tab and asks if they want to save their work.
     */
//    void on_tabWidget_tabCloseRequested(int index);

    void on_tabWidget_tabBarClicked(int index);

    void on_actionView_Rendered_HTML_triggered();

    void on_actionSave_triggered();

    void on_actionFormat_Text_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_actionUnderline_triggered();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_fontSizeComboBox_currentIndexChanged(int index);

    void on_fontSizeComboBox_activated(const QString &arg1);

    void on_actionOptions_triggered();

    void on_actionCenter_triggered();

    void on_actionAlign_Left_triggered();

    void on_actionAlign_Right_triggered();

    void on_actionSave_2_triggered();

    void on_actionBullets_triggered();

    void on_actionNumbering_triggered();

    void on_actionFind_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    static const qint8 numCopies = 3; /*!< Size of the paste storage. */
    QMimeData savedCopy[numCopies]; /*!< The paste storage which allows for multiple copied things to be stored. */

    int track; /*!< keeps track of the next slot to be updated in savedCopy. */
    QMenu *pasteMenu; /*!< The menu for the context menu. */
    int zoom; /*!< Keeps track of the standard for zooming. */

    QFileSystemWatcher *fileWatcher; /*!< Keeps track of events such as file renaming and directory movements. */
    OptionsWindow* optionsWindow; /*!< Options window pointer for when we need to open it. */

    SettingsManager* settings; /*!< Settings Manager singleton reference. */

    QDockWidget* dock; /*!< Used for creating docking widgets. */

    QVector<CustomTabWidget*> *tabWidgets; /*!< Holds all the tab widgets in the screen. */
};
#endif // MAINWINDOW_H
