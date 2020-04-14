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
#include "customdockwidget.h"

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


    //! Getter
    /*!
        Gets the current texttabwidget associated with the current widget (like docking widget etc).
     */
    TextTabWidget* getCurrentTabWidget();


    //! Window name setter
    /*!
        Sets the window name to the current active tab file path
     */
    void setWindowToFileName(int index);

    //! Saved Tab
    /*!
        Removes the * from tab showing they saved
     */
    void markTextTabAsClean(const QString& newPath);

    //! Window name setter
    /*!
        Shows the printer preview to the user showing what it'll look like.
     */
    void printPreview(QPrinter *printer);


    //! Font Information from tab
    /*!
        Gets font information when a tab is being transferred like status of menu bar.
     */
    QFont getFontInformation();

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

    //! When the user right clicks in the main window.
    /*!
        Shows a custom context menu at the click location that can undo, redo, cut, copy, paste, and zoom.
     */
    void showContextMenu(const QPoint&);

    //! Used for closing tabs from other docking widgets
    /*!
        Calls the standard Main windows private saving function.
     */
    void save();

    //! Whenever something in the current file is changed
    /*!
        Adds an asterisk into the tab name so that the user knows that the file is changed.
     */
    void fileChanged();

    //! UI Getter
    /*!
        Returns access to the ui to get information from other classes
     */
    Ui::MainWindow get_UI();
protected:

    //! Drag event
    /*!
        Handle when a drag enter event happens.
     */
    void dragEnterEvent(QDragEnterEvent *event);

    //! Drop event
    /*!
        Handles whether or not a new widget is shown, added on to, and if we accept the tab transfer
     */
    void dropEvent(QDropEvent *event);
private slots:
    //! Creates a text edit tab.
    /*!
        Opens a new blank tab for creating a new file.
     */
    void on_actionNew_triggered();

    //! Load saved files.
    /*!
        Opens a directory window to select previously saved files. These files will occupy their own tabs.
     */
    void on_actionOpen_triggered();

    //! Save files.
    /*!
        Opens a directory window to save the file occupying the current tab.
     */
    void on_actionSave_as_triggered();

    //! Printing files.
    /*!
        Opens a printer preview window for setting your printing prefernces, to then print the file.
     */
    void on_actionPrint_triggered();

    //! Exiting.
    /*!
        Closing the application.
     */
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

    //! Builds a context menu for pasting
    /*!
        Executes a context menu that shows the current things being stored in the multi paste storage and allows them to be clicked for pasting.
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

    //! Tab bar clicked
    /*!
      Handles setting up the connection for special context menu whenever a tab bar is clicked
     */
    void on_tabWidget_tabBarClicked(int index);

    //! Save
    /*!
        Saves the file to the window file path, if its new it'll call Save As
     */
    void on_actionSave_triggered();

    void on_actionFormat_Text_triggered();

    //! Font Style.
    /*!
        Sets selected text to be Bolded and continues with Bolded style until turned off.
        If no text is selected then it will just continue with Bolded.
        Can be used with other Font Style. (i.e. Italic, Underline)
     */
    void on_actionBold_triggered();

    //! Font Style.
    /*!
        Sets selected text to be Italic and continues with Italic style until turned off.
        If no text is selected then it will just continue with Italic.
        Can be used with other Font Style. (i.e. Bold, Underline)
    */
    void on_actionItalic_triggered();

    //! Font Style.
    /*!
        Sets selected text to be Underlined and continues with Underlinned style until turned off.
        If no text is selected then it will just continue with Underline.
        Can be used with other Font Style. (i.e. Bold, Italic)
    */
    void on_actionUnderline_triggered();

    //! Font Family.
    /*!
        Sets the selected text as the desired Font Family and continues with that setting.
        If no text is selected then it will just continue with the new Font Family.
    */
    void on_fontComboBox_currentFontChanged(const QFont &f);

    //! Font Size.
    /*!
        Sets the selected text as the desired Font Size and continues with that setting.
        If no text is selected then it will just continue with the new Font size.
    */
    void on_fontSizeComboBox_currentIndexChanged(int index);

    //! Font Size.
    /*!
        Sets the selected text as the desired Font Size and continues with that setting.
        If no text is selected then it will just continue with the new Font size.
    */
    void on_fontSizeComboBox_activated(const QString &arg1);

    //! Option Window
    /*!
        Opens the option window once the menu action is triggered.
    */
    void on_actionOptions_triggered();

    //! Alignment.
    /*!
        Sets the selected text block to Center Alignement and continues.
        If no text is selected then it will just continue with Center Alignment.
    */
    void on_actionCenter_triggered();

    //! Alignment.
    /*!
        Sets the selected text block to Left Alignement and continues.
        If no text is selected then it will just continue with Left Alignment.
    */
    void on_actionAlign_Left_triggered();

    //! Alignment.
    /*!
        Sets the selected text block to Right Alignement and continues.
        If no text is selected then it will just continue with Right Alignment.
    */
    void on_actionAlign_Right_triggered();

    void on_actionSave_2_triggered();

    //! Listing Format.
    /*!
        Creates a Listing Format with Bulleting system.
    */
    void on_actionBullets_triggered();

    //! Listing Format.
    /*!
        Creates a Listing Format with Numbering system.
    */
    void on_actionNumbering_triggered();

    //! Find/Replacer Action
    /*!
        Opens the Find and replacer text edits in the currently editing document.
     */
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

    CustomDockWidget* dock; /*!< Used for creating docking widgets. */

    QVector<CustomTabWidget*> *tabWidgets; /*!< Holds all the tab widgets in the screen. */
};
#endif // MAINWINDOW_H
