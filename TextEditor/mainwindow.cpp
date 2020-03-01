#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "texttabwidget.h"

#include <QTabBar>
#include <QDockWidget>
#include <QDebug>
//#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuBar()->setNativeMenuBar(false);

    ui->tabWidget->setMovable(false);
    ui->tabWidget->setTabsClosable(true);

    splitter = new QSplitter();

    ui->tabWidget->removeTab(0);
    ui->tabWidget->insertTab(0, new TextTabWidget(), "New Tab");

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setWindowToFileName);

    ui->tabWidget->setCurrentIndex(0);
    TextTabWidget* textTabWidget = getCurrentTabWidget();

    //Make sure text edit is in focus incase they just start typing right away
    textTabWidget->getTextEdit()->setFocus();

    track = 1;
    textTabWidget->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(textTabWidget->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    savedCopy[0] = QApplication::clipboard()->text();

    zoom = 0;
    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);

    QObject::connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &MainWindow::clipboard_changed);

    QShortcut *zoomIn = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus), this);

    QObject::connect(zoomIn,
                     &QShortcut::activated,
                     this,
                     &MainWindow::on_actionZoom_in_triggered);

    QShortcut *zoomOut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus), this);

    QObject::connect(zoomOut,
                     &QShortcut::activated,
                     this,
                     &MainWindow::on_actionZoom_Out_triggered);


    ui->tabWidget->setTabText(1, "+");

    QTabBar* tabBar = ui->tabWidget->tabBar();

    tabBar->tabButton(1, QTabBar::RightSide)->deleteLater();
    tabBar->setTabButton(1, QTabBar::RightSide, 0);

    fileWatcher = new QFileSystemWatcher();

    QObject::connect(fileWatcher,
                     &QFileSystemWatcher::fileChanged,
                     this,
                     &MainWindow::markTextTabAsClean);

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
    ui->tabWidget->insertTab(ui->tabWidget->count() - 1, new TextTabWidget() , "New Tab");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 2);
    getCurrentTabWidget()->getTextEdit()->setText(QString());
    getCurrentTabWidget()->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(getCurrentTabWidget()->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
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
    TextTabWidget* textTabWidget = getCurrentTabWidget();

    //Current Tab is occupied so make a new tab, set its name, and then then carry on with setting the text
    if(textTabWidget->getTabFileName() != "New File.txt")
    {
        on_actionNew_triggered();
        textTabWidget = getCurrentTabWidget();
        tabIndex = ui->tabWidget->currentIndex();
    }

    textTabWidget->setTabsFileName(fileName);

    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();

    textTabWidget->setTextEditText(text);

    QFileInfo fileInfo(fileName);
    ui->tabWidget->setTabText(tabIndex, fileInfo.fileName()); //calls setTabText(index of tab => int, name of file => QString);

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

    getCurrentTabWidget()->setTabsFileName(fileName);
    QFileInfo fileInfo(fileName);

    int tabIndex = ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(tabIndex, fileInfo.fileName()); //calls setTabText(index of tab => int, name of file => QString);

    QTextStream out(&file);
    QString text = getCurrentTabWidget()->getTextEdit()->toPlainText();
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
    getCurrentTabWidget()->getTextEdit()->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::fileChanged()
{
    QTabWidget* tab = ui->tabWidget;
    int index = tab->currentIndex();
    if(tab->tabText(index).back() != '*')
        tab->setTabText(index, tab->tabText(index) + "*");
}

void MainWindow::on_actionCopy_triggered()
{
    TextTabWidget* textTabWidget = getCurrentTabWidget();

    if(textTabWidget->getTextEdit()->textCursor().hasSelection())
    {
        QString text = textTabWidget->getTextEdit()->textCursor().selectedText();

        savedCopy[track] = text;
        if(track >= 2)
            track = 0;
        else
            track++;

   }
   textTabWidget->getTextEdit()->copy();
}

void MainWindow::on_actionCut_triggered()
{
    TextTabWidget* textTabWidget = getCurrentTabWidget();

    if(textTabWidget->getTextEdit()->textCursor().hasSelection())
    {
        QString text = textTabWidget->getTextEdit()->textCursor().selectedText();

        savedCopy[track] = text;
        if(track >= 2)
            track = 0;
        else
            track++;

   }

   textTabWidget->getTextEdit()->cut();
}

void MainWindow::clipboard_changed()
{
    savedCopy[track] = QApplication::clipboard()->text();
    if(track >= 2)
        track = 0;
    else
        track++;
}


void MainWindow::showContextPasteMenu(const QPoint& pos)
{
    QPoint globalPos = getCurrentTabWidget()->getTextEdit()->mapToGlobal(pos);

    QMenu *pasteMenu = new QMenu(this);
    pasteMenu->addAction(QString(savedCopy[0].left(20) + ((savedCopy[0].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));

    if(savedCopy[1] != "")
        pasteMenu->addAction(QString(savedCopy[1].left(20) + ((savedCopy[1].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2] != "")
        pasteMenu->addAction(QString(savedCopy[2].left(20) + ((savedCopy[2].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_4_triggered()));

    pasteMenu->exec(globalPos);

}

void MainWindow::on_actionPaste_triggered()
{
    showContextPasteMenu(cursor().hotSpot());
    //ui->textEdit->textCursor().insertText(savedCopy[0]);
    //pasteMenu->popup(ui->textEdit->textCursor().position());
}

void MainWindow::on_actionPaste_2_triggered()
{
    getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[0]);
}

void MainWindow::on_actionPaste_3_triggered()
{
    getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[1]);
}

void MainWindow::on_actionPaste_4_triggered()
{
    getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[2]);
}

void MainWindow::on_actionUndo_triggered()
{
    getCurrentTabWidget()->getTextEdit()->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    getCurrentTabWidget()->getTextEdit()->redo();
}

void MainWindow::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = getCurrentTabWidget()->getTextEdit()->mapToGlobal(pos);

    QMenu *pasteMenu = new QMenu(), *rightClick = new QMenu(this);

    rightClick->addAction(QString("Undo"), this, SLOT(on_actionUndo_triggered()));
    rightClick->addAction(QString("Redo"), this, SLOT(on_actionRedo_triggered()));
    rightClick->addSeparator();
    rightClick->addAction(QString("Cut"), this, SLOT(on_actionCut_triggered()));
    rightClick->addAction(QString("Copy"), this, SLOT(on_actionCopy_triggered()));

    pasteMenu->setTitle(QString("Paste"));
    pasteMenu->addAction(QString(savedCopy[0].left(20) + ((savedCopy[0].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));

    if(savedCopy[1] != "")
        pasteMenu->addAction(QString(savedCopy[1].left(20) + ((savedCopy[1].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2] != "")
        pasteMenu->addAction(QString(savedCopy[2].left(20) + ((savedCopy[2].size() > 20) ? " ..." : "")), this, SLOT(on_actionPaste_4_triggered()));

    rightClick->addMenu(pasteMenu);

    rightClick->addSeparator();
    rightClick->addAction(QString("Zoom In"), this, SLOT(on_actionZoom_in_triggered()));
    rightClick->addAction(QString("Zoom Out"), this, SLOT(on_actionZoom_Out_triggered()));
    rightClick->addAction(QString("Zoom Standard"), this, SLOT(on_actionZoom_Standard_triggered()));

    rightClick->exec(globalPos);

}

void MainWindow::on_actionZoom_in_triggered()
{
    QTextEdit* textEdit = getCurrentTabWidget()->getTextEdit();
    zoom++;

    textEdit->zoomIn();
}

void MainWindow::on_actionZoom_Out_triggered()
{
    QTextEdit* textEdit = getCurrentTabWidget()->getTextEdit();
    zoom--;

    textEdit->zoomOut();
}

void MainWindow::on_actionZoom_Standard_triggered()
{
    QTextEdit* textEdit = getCurrentTabWidget()->getTextEdit();
    zoom = -zoom;

    textEdit->zoomIn(zoom);

    zoom = 0;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if(ui->tabWidget->tabText(index).back() == '*')
    {
        int clicked = QMessageBox::warning(this, "Save?", "Would you like to save the file?", QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::Close);
        if(clicked == QMessageBox::Ok)
        {
            on_actionSave_triggered();
            qDebug() << "Closing " + QString::number(index);
            ui->tabWidget->removeTab(index);
        }
        else if(clicked == QMessageBox::Close)
        {
            qDebug() << "Closing " + QString::number(index);
            ui->tabWidget->removeTab(index);
        }
    }
    else
    {
        qDebug() << "Closing " + QString::number(index);
        ui->tabWidget->removeTab(index);
    }

}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << index;
    //This will make sure to insert where the last tab is and they cannot move it or itll make a new tab
    if(index == ui->tabWidget->count() - 1)
    {
        ui->tabWidget->insertTab(index, new TextTabWidget(), "New tab");
        ui->tabWidget->setCurrentIndex(index);
    }

    getCurrentTabWidget()->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(getCurrentTabWidget()->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
}

void MainWindow::on_actionView_Rendered_HTML_triggered()
{
//    QWebEngineView *view = new QWebEngineView();

//    view->load(QUrl::fromLocalFile(getCurrentTabWidget()->getTabFileName()));
//    view->show();
}

void MainWindow::on_actionSplit_Dock_Horizontally_triggered()
{
    splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
}

void MainWindow::on_actionSave_triggered()
{
    int index = ui->tabWidget->currentIndex();
    TextTabWidget* textTabWidget = getCurrentTabWidget();
    qDebug() << textTabWidget->getTabFileName();

    //If the tabwidget has an actual path thats not just New File.txt (default name) then actually save it using that name
    if(textTabWidget->getTabFileName() != "New File.txt")
    {
        QFile file(textTabWidget->getTabFileName());
        if(ui->tabWidget->tabText(index).back() == '*')
            ui->tabWidget->setTabText(index, ui->tabWidget->tabText(index).left(ui->tabWidget->tabText(index).size() - 1));

        if(file.open(QFile::WriteOnly))
        {
        QTextStream out(&file);
        QString text = textTabWidget->getTextEdit()->toPlainText();
        out << text;
        }
        file.close();

    }
    else //File hasnt been made yet so ask them for a name!
    {
        on_actionSave_as_triggered();
    }

}

void MainWindow::markTextTabAsClean(const QString &newPath)
{
    int index = ui->tabWidget->currentIndex();
    if(!fileWatcher->files().contains(newPath))
        fileWatcher->addPath(newPath);
    if(ui->tabWidget->tabText(index).back() == '*')
        ui->tabWidget->setTabText(index, ui->tabWidget->tabText(index).left(ui->tabWidget->tabText(index).size() - 1));
    qDebug() << "file changed"; //file saved take away the * showing it saved

}

void MainWindow::setWindowToFileName(int index)
{
    qDebug() << "Clicked on tab " + QString::number(index);

    setWindowTitle(getCurrentTabWidget()->getTabFileName());
}

void MainWindow::on_actionFormat_Text_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);

    if(ok)
    {
        getCurrentTabWidget()->getTextEdit()->setFont(font);
    }
    else
        return;
}

void MainWindow::on_actionBold_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
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
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
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
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
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

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;
    format.setFont(f);

    cursor.mergeCharFormat(format);//Change font-family
}

void MainWindow::on_fontSizeComboBox_activated(const QString &arg1)
{
    QString size = ui->fontSizeComboBox->currentText();
    QFont font;
    font.setPointSize(size.toInt());

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;
    format.setFont(font);

    cursor.mergeCharFormat(format);//change font size
}

void MainWindow::on_fontSizeComboBox_currentIndexChanged(int index)
{
    int size = ui->fontSizeComboBox->currentText().toInt();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;

    QFont font;
    font.setPointSize(size);

    format.setFont(font);

    cursor.mergeCharFormat(format);
}

TextTabWidget* MainWindow::getCurrentTabWidget()
{
    int tabIndex = ui->tabWidget->currentIndex();
    TextTabWidget* textTabWidget = (TextTabWidget*) ui->tabWidget->widget(tabIndex);

    return textTabWidget;
}
