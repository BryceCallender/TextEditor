#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ui->tabWidget->setMovable(true);
    ui->tabWidget->setTabsClosable(true);

    setAcceptDrops(true);

    tabWidgets = new QVector<CustomTabWidget*>();
    tabWidgets->push_back(ui->tabWidget);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setWindowToFileName);

    setCentralWidget(ui->tabWidget);

    ui->tabWidget->setCurrentIndex(0);
    TextTabWidget* textTabWidget = getCurrentTabWidget();

    //Make sure text edit is in focus incase they just start typing right away
    textTabWidget->getTextEdit()->setFocus();

    track = 0;
    textTabWidget->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(textTabWidget->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    clipboard_changed();

    zoom = 8;
    QObject::connect(textTabWidget->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);

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

    settings = SettingsManager::getInstance();
    setGeometry(settings->getValue("ui/geometry").toRect());

    //Sets values to the saved settings
    ui->fontComboBox->setCurrentFont(settings->getValue("text/fontFamily").value<QFont>());
    ui->fontSizeComboBox->setCurrentText(settings->getValue("text/fontSize").toString());

    undoStack = new QUndoStack(this);

    // create shortcut
    QShortcut *saveShortcut = new QShortcut(QKeySequence::Save, this);

    QObject::connect(saveShortcut,
                     &QShortcut::activated,
                     this,
                     &MainWindow::on_actionSave_triggered);

    // create shortcut
    QShortcut *saveAsShortcut = new QShortcut(QKeySequence::SaveAs, this);

    QObject::connect(saveAsShortcut,
                     &QShortcut::activated,
                     this,
                     &MainWindow::on_actionSave_as_triggered);

    // create shortcut
    QShortcut *printShortcut = new QShortcut(QKeySequence::Print, this);

    QObject::connect(printShortcut,
                     &QShortcut::activated,
                     this,
                     &MainWindow::on_actionPrint_triggered);

     ui->tabWidget->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);

    currentWidget->insertTab(currentWidget->count() - 1, new TextTabWidget(currentWidget) , "New Tab");
    currentWidget->setCurrentIndex(currentWidget->count() - 2);
    getCurrentTabWidget()->getTextEdit()->setText(QString());
    getCurrentTabWidget()->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(getCurrentTabWidget()->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Open the file");

    for(QString fileName: fileNames)
    {
        QFile file(fileName);
        currentFile = fileName;
        if(!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Warning", "Cannot open " + fileName + ": " + file.errorString());
            return;
        }

        TextTabWidget* textTabWidget = getCurrentTabWidget();
        CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
        int tabIndex = currentWidget->currentIndex();

        //Current Tab is occupied so make a new tab, set its name, and then then carry on with setting the text
        if(textTabWidget->getTabFileName() != "New File.txt" || currentWidget->count() == 1)
        {
            on_actionNew_triggered();
            tabIndex = currentWidget->currentIndex();
            textTabWidget = getCurrentTabWidget();
        }

        textTabWidget->setTabsFileName(fileName);

        setWindowTitle(fileName);
        QTextStream in(&file);
        QString text = in.readAll();
        if(fileName.contains(".txt"))
        {
            textTabWidget->getTextEdit()->setHtml(text);
        }
        else
        {
            textTabWidget->getTextEdit()->setText(text);
        }

        QFileInfo fileInfo(fileName);
        currentWidget->setTabText(tabIndex, fileInfo.fileName()); //calls setTabText(index of tab => int, name of file => QString);

        file.close();
    }
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

    TextTabWidget* textTabWidget = getCurrentTabWidget();
    textTabWidget->setTabsFileName(fileName);
    QFileInfo fileInfo(fileName);

    CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
    int tabIndex = currentWidget->currentIndex();

    currentWidget->setTabText(tabIndex, fileInfo.fileName()); //calls setTabText(index of tab => int, name of file => QString);

    QTextStream out(&file);
    if(fileName.contains(".txt"))
    {
        QString text = textTabWidget->getTextEdit()->toHtml();
        out << text;
    }
    else
    {
        QString text = textTabWidget->getTextEdit()->toPlainText();
        out << text;
    }
    file.close();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPrinterName("Printer Name");

    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested,
                this, &MainWindow::printPreview);
    if(preview.exec() == QDialog::Rejected)
    {
        printer.abort();
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }
    //preview.exec();
}

void MainWindow::printPreview(QPrinter *printer)
{
    getCurrentTabWidget()->getTextEdit()->print(printer);
}

QFont MainWindow::getFontInformation()
{
    QFont fontInfo;

    fontInfo.setBold(ui->actionBold->isChecked());
    fontInfo.setItalic(ui->actionItalic->isChecked());
    fontInfo.setUnderline(ui->actionUnderline->isChecked());

    fontInfo.setPointSize(ui->fontSizeComboBox->currentText().toInt());
    fontInfo.setFamily(ui->fontComboBox->currentFont().family());

    return fontInfo;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::fileChanged()
{
    CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
    int index = currentWidget->currentIndex();
    if(currentWidget->tabText(index).back() != '*')
        currentWidget->setTabText(index, currentWidget->tabText(index) + "*");
}
void MainWindow::save()
{
    on_actionSave_triggered();
}

void MainWindow::on_actionCopy_triggered()
{
    TextTabWidget* textTabWidget = getCurrentTabWidget();
/*
    if(textTabWidget->getTextEdit()->textCursor().hasSelection())
    {
        QString text = textTabWidget->getTextEdit()->textCursor().selectedText();

        savedCopy[track] = text;
        if(track >= 2)
            track = 0;
        else
            track++;

   }*/

   textTabWidget->getTextEdit()->copy();
}

void MainWindow::on_actionCut_triggered()
{
    TextTabWidget* textTabWidget = getCurrentTabWidget();
/*
    if(textTabWidget->getTextEdit()->textCursor().hasSelection())
    {
        QString text = textTabWidget->getTextEdit()->textCursor().selectedText();

        savedCopy[track] = text;
        if(track >= 2)
            track = 0;
        else
            track++;

   }
*/
   textTabWidget->getTextEdit()->cut();
}

void MainWindow::clipboard_changed()
{
    if(!QApplication::clipboard()->image().isNull())
    {
        savedCopy[track].setImageData(QApplication::clipboard()->image());
        if(track >= 2)
            track = 0;
        else
            track++;
    }
    else if(QApplication::clipboard()->text() != "")
    {
        savedCopy[track].setText(QApplication::clipboard()->text());
        if(track >= 2)
            track = 0;
        else
            track++;
    }
}


void MainWindow::showContextPasteMenu(const QPoint& pos)
{
    QPoint globalPos = getCurrentTabWidget()->getTextEdit()->mapToGlobal(pos);

    QMenu *pasteMenu = new QMenu(this);
    QAction *tool;
    if(savedCopy[0].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[0].text().left(40) + ((savedCopy[0].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[0].text());
    }
    else if(savedCopy[0].hasImage())
        pasteMenu->addAction("Copied Image 0", this, SLOT(on_actionPaste_2_triggered()));
    if(savedCopy[1].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[1].text().left(40) + ((savedCopy[1].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[1].text());
    }
    else if(savedCopy[1].hasImage())
        pasteMenu->addAction("Copied Image 1", this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[2].text().left(40) + ((savedCopy[2].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[2].text());
    }
    else if(savedCopy[2].hasImage())
        pasteMenu->addAction("Copied Image 2", this, SLOT(on_actionPaste_4_triggered()));

    pasteMenu->setToolTipsVisible(true);

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
    if(savedCopy[0].hasText())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[0].text());
    else if(savedCopy[0].hasImage())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertImage(savedCopy[0].imageData().value<QImage>());
}

void MainWindow::on_actionPaste_3_triggered()
{
    if(savedCopy[1].hasText())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[1].text());
    else if(savedCopy[1].hasImage())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertImage(savedCopy[1].imageData().value<QImage>());
}

void MainWindow::on_actionPaste_4_triggered()
{
    if(savedCopy[2].hasText())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertText(savedCopy[2].text());
    else if(savedCopy[2].hasImage())
        getCurrentTabWidget()->getTextEdit()->textCursor().insertImage(savedCopy[2].imageData().value<QImage>());
}

void MainWindow::on_actionPaste_5_triggered()
{
    on_actionPaste_triggered();
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
    QAction *tool;

    rightClick->addAction(QString("Undo"), this, SLOT(on_actionUndo_triggered()));
    rightClick->addAction(QString("Redo"), this, SLOT(on_actionRedo_triggered()));
    rightClick->addSeparator();
    rightClick->addAction(QString("Cut"), this, SLOT(on_actionCut_triggered()));
    rightClick->addAction(QString("Copy"), this, SLOT(on_actionCopy_triggered()));

    pasteMenu->setTitle(QString("Paste"));

    if(savedCopy[0].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[0].text().left(40) + ((savedCopy[0].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[0].text());
        //QObject::connect(tool, &QAction::hovered, tool,  &QAction::toolTip);
    }
    else if(savedCopy[0].hasImage())
        pasteMenu->addAction("Copied Image 0", this, SLOT(on_actionPaste_2_triggered()));
    if(savedCopy[1].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[1].text().left(40) + ((savedCopy[1].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[1].text());
    }
    else if(savedCopy[1].hasImage())
        pasteMenu->addAction("Copied Image 1", this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2].hasText()){
        tool = pasteMenu->addAction(QString(savedCopy[2].text().left(40) + ((savedCopy[2].text().size() > 40) ? " ..." : "")), this, SLOT(on_actionPaste_2_triggered()));
        tool->setToolTip(savedCopy[2].text());
    }
    else if(savedCopy[2].hasImage())
        pasteMenu->addAction("Copied Image 2", this, SLOT(on_actionPaste_4_triggered()));

    pasteMenu->setToolTipsVisible(true);

    rightClick->addMenu(pasteMenu);

    rightClick->addSeparator();
    rightClick->addAction(QString("Zoom In"), this, SLOT(on_actionZoom_in_triggered()));
    rightClick->addAction(QString("Zoom Out"), this, SLOT(on_actionZoom_Out_triggered()));
    rightClick->addAction(QString("Zoom Standard"), this, SLOT(on_actionZoom_Standard_triggered()));

    rightClick->exec(globalPos);

}

void MainWindow::on_actionZoom_in_triggered()
{
    QFont font = getCurrentTabWidget()->getTextEdit()->currentFont();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;

    font.setPointSize(font.pointSize() + 1);
    font.setFamily(getCurrentTabWidget()->getTextEdit()->fontFamily());
    format.setFont(font);
    QTextDocumentFragment selection = cursor.selection();

    getCurrentTabWidget()->getTextEdit()->setCurrentFont(font);

    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(format);
    cursor.clearSelection();


}

void MainWindow::on_actionZoom_Out_triggered()
{
    QFont font = getCurrentTabWidget()->getTextEdit()->currentFont();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;

    font.setPointSize(font.pointSize() - 1);
    font.setFamily(getCurrentTabWidget()->getTextEdit()->fontFamily());
    format.setFont(font);
    QTextDocumentFragment selection = cursor.selection();

    getCurrentTabWidget()->getTextEdit()->setCurrentFont(font);

    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(format);
    cursor.clearSelection();

}

void MainWindow::on_actionZoom_Standard_triggered()
{
    QFont font = getCurrentTabWidget()->getTextEdit()->currentFont();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;

    font.setPointSize(zoom);
    font.setFamily(getCurrentTabWidget()->getTextEdit()->fontFamily());
    format.setFont(font);
    QTextDocumentFragment selection = cursor.selection();

    getCurrentTabWidget()->getTextEdit()->setCurrentFont(font);

    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(format);
    cursor.clearSelection();
}

//void MainWindow::on_tabWidget_tabCloseRequested(int index)
//{
//    if(ui->tabWidget->tabText(index).back() == '*')
//    {
//        int clicked = QMessageBox::warning(this, "Save?", "Would you like to save the file?", QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::No);
//        if(clicked == QMessageBox::Ok)
//        {
//            on_actionSave_triggered();
//            qDebug() << "Closing " + QString::number(index);
//            ui->tabWidget->removeTab(index);
//        }
//        else if(clicked == QMessageBox::No)
//        {
//            qDebug() << "Closing " + QString::number(index);
//            ui->tabWidget->removeTab(index);
//        }
//    }
//    else
//    {
//        qDebug() << "Closing " + QString::number(index);
//        ui->tabWidget->removeTab(index);
//    }

//    ui->tabWidget->setCurrentIndex(index - 1);
//}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    //This will make sure to insert where the last tab is and they cannot move it or itll make a new tab
//    if(index == ui->tabWidget->count() - 1)
//    {
//        ui->tabWidget->insertTab(index, new TextTabWidget(), "New Tab");
//        ui->tabWidget->setCurrentIndex(index);
//    }

    getCurrentTabWidget()->getTextEdit()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(getCurrentTabWidget()->getTextEdit(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
    QObject::connect(getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
}

void MainWindow::on_actionSave_triggered()
{
    TextTabWidget* textTabWidget = getCurrentTabWidget();
    CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
    int index = currentWidget->currentIndex();

    //If the tabwidget has an actual path thats not just New File.txt (default name) then actually save it using that name
    if(textTabWidget->getTabFileName() != "New File.txt")
    {
        QFile file(textTabWidget->getTabFileName());
        if(currentWidget->tabText(index).back() == '*')
            currentWidget->setTabText(index, currentWidget->tabText(index).left(currentWidget->tabText(index).size() - 1));

        if(file.open(QFile::WriteOnly))
        {
            QTextStream out(&file);
            if(textTabWidget->getTabFileName().contains(".txt"))
            {
                QString text = textTabWidget->getTextEdit()->toHtml();
                out << text;
            }
            else
            {
                QString text = textTabWidget->getTextEdit()->toPlainText();
                out << text;
            }

        }
        file.close();

    }
    else //File hasnt been made yet so ask them for a name!
    {
        on_actionSave_as_triggered();
    }

}

 void MainWindow::on_actionSave_2_triggered()
 {
     on_actionSave_triggered();
 }

void MainWindow::markTextTabAsClean(const QString &newPath)
{
    CustomTabWidget* currentWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
    int index = currentWidget->currentIndex();
    if(!fileWatcher->files().contains(newPath))
        fileWatcher->addPath(newPath);
    if(currentWidget->tabText(index).back() == '*')
        currentWidget->setTabText(index, currentWidget->tabText(index).left(currentWidget->tabText(index).size() - 1));
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
    QTextCharFormat format;


    if(ok)
    {
        qDebug() << font.strikeOut();
        format.setFont(font);

        if(font.pointSize() != ui->fontSizeComboBox->currentData().toInt())
        {
            int index = ui->fontSizeComboBox->findText(QString::number(font.pointSize()));
            ui->fontSizeComboBox->setCurrentIndex(index);

        }

        if(font.family() != ui->fontComboBox->currentText())
        {
            int index = ui->fontComboBox->findText(font.family());
            ui->fontComboBox->setCurrentIndex(index);

        }

        getCurrentTabWidget()->getTextEdit()->setFont(font);
        //getCurrentTabWidget()->getTextEdit()->textCursor().setCharFormat(format);

    }
    else
        return;
}

void MainWindow::on_actionBold_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();

    if(ui->actionBold->isChecked())
        getCurrentTabWidget()->getTextEdit()->setFontWeight(QFont::Bold);
    else
        getCurrentTabWidget()->getTextEdit()->setFontWeight(QFont::Normal);

}

void MainWindow::on_actionItalic_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();

    QTextCharFormat format;
    if(ui->actionItalic->isChecked())
        getCurrentTabWidget()->getTextEdit()->setFontItalic(true);
    else
        getCurrentTabWidget()->getTextEdit()->setFontItalic(false);

}

void MainWindow::on_actionUnderline_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();

    QTextCharFormat format;
    if(ui->actionUnderline->isChecked())
        getCurrentTabWidget()->getTextEdit()->setFontUnderline(true);
    else
        getCurrentTabWidget()->getTextEdit()->setFontUnderline(false);
}

void MainWindow::on_actionBullets_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();

    QTextListFormat listFormat = QTextListFormat();
    if (cursor.currentList()) {
        listFormat = cursor.currentList()->format();
        listFormat.setIndent(listFormat.indent() + 1);
     }

     listFormat.setStyle(QTextListFormat::Style::ListDisc);
     cursor.insertList(listFormat);

//    if(ui->actionBullets->isChecked())
//    {
//        if(ui->actionNumbering->isChecked())
//        {
//            ui->actionNumbering->setChecked(false);
//        }
//        cursor.beginEditBlock();
//        QTextBlockFormat blockFmt = cursor.blockFormat();
//        QTextListFormat listFmt;

//        if (cursor.currentList())
//        {
//            listFmt = cursor.currentList()->format();
//        }
//        else
//        {
//            listFmt.setIndent(blockFmt.indent() + (settings->getValue("text/tabLength").toInt()/4));
//            blockFmt.setIndent(0);
//            cursor.setBlockFormat(blockFmt);
//        }

//        listFmt.setStyle(QTextListFormat::ListDisc);
//        cursor.createList(listFmt);
//        cursor.endEditBlock();
//    }
//    else
//    {
//        QTextBlockFormat bfmt;
//        bfmt.setObjectIndex(0);
//        cursor.mergeBlockFormat(bfmt);
//    }

}

void MainWindow::on_actionNumbering_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();

    QTextListFormat listFormat = QTextListFormat();
    if (cursor.currentList()) {
        listFormat = cursor.currentList()->format();
        listFormat.setIndent(listFormat.indent() + 1);
     }

     listFormat.setStyle(QTextListFormat::Style::ListDecimal);
     cursor.insertList(listFormat);

//    if(ui->actionNumbering->isChecked())
//    {
//        if(ui->actionBullets->isChecked())
//        {
//            ui->actionBullets->setChecked(false);
//        }
//        cursor.beginEditBlock();
//        QTextBlockFormat blockFmt = cursor.blockFormat();
//        QTextListFormat listFmt;

//        if (cursor.currentList())
//        {
//            listFmt = cursor.currentList()->format();
//        }
//        else
//        {
//            listFmt.setIndent(blockFmt.indent() + (settings->getValue("text/tabLength").toInt()/4));
//            blockFmt.setIndent(0);
//            cursor.setBlockFormat(blockFmt);
//        }

//        listFmt.setStyle(QTextListFormat::ListDecimal);
//        cursor.createList(listFmt);
//        cursor.endEditBlock();
//    }
//    else
//    {
//        QTextBlockFormat bfmt;
//        bfmt.setObjectIndex(0);
//        cursor.mergeBlockFormat(bfmt);
//    }
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    //QFont font = ui->fontComboBox->currentFont();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();


    if(cursor.hasSelection())
    {
        QTextCharFormat format = cursor.charFormat();
        format.setFontFamily(f.family());
        cursor.mergeCharFormat(format);//Change font-family
    }
    else
    {
        QString size = ui->fontSizeComboBox->currentText();
        QTextCharFormat format;
        format.setFont(f);
        format.setFontPointSize(size.toInt());
        getCurrentTabWidget()->getTextEdit()->mergeCurrentCharFormat(format);
    }

}

void MainWindow::on_fontSizeComboBox_activated(const QString &arg1)
{
    Q_UNUSED(arg1);

    QString size = ui->fontSizeComboBox->currentText();
    QFont font;
    font.setPointSize(size.toInt());
    font.setFamily(getCurrentTabWidget()->getTextEdit()->fontFamily());

    zoom = size.toInt();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;
    format.setFont(font);

    if(cursor.hasSelection())
    {
        cursor.mergeCharFormat(format);//change font size
    }
    else
    {
        getCurrentTabWidget()->getTextEdit()->setCurrentFont(font);
    }

}

void MainWindow::on_fontSizeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    int size = ui->fontSizeComboBox->currentText().toInt();

    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextCharFormat format;

    zoom = size;

    QFont font;
    font.setPointSize(size);
    font.setFamily(getCurrentTabWidget()->getTextEdit()->fontFamily());
    format.setFont(font);

    if(cursor.hasSelection())
    {
        cursor.mergeCharFormat(format);
    }
    else
    {
        getCurrentTabWidget()->getTextEdit()->setCurrentFont(font);
    }
}

TextTabWidget* MainWindow::getCurrentTabWidget()
{
    qDebug() << "Getting TextEdit for tab index" << CustomTabWidget::currentSelectedTabIndex;
    CustomTabWidget* customTabWidget = tabWidgets->at(CustomTabWidget::currentSelectedTabIndex);
    TextTabWidget* textTabWidget = (TextTabWidget*) customTabWidget->widget(customTabWidget->currentIndex());

    return textTabWidget;
}

void MainWindow::on_actionOptions_triggered()
{
    optionsWindow = new OptionsWindow(this);

    optionsWindow->show();
}

void MainWindow::on_actionAlign_Left_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignLeft);
    cursor.mergeBlockFormat(textBlockFormat);

    getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
}

void MainWindow::on_actionCenter_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignCenter);
    cursor.mergeBlockFormat(textBlockFormat);

    getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
}

void MainWindow::on_actionAlign_Right_triggered()
{
    QTextCursor cursor = getCurrentTabWidget()->getTextEdit()->textCursor();
    QTextBlockFormat textBlockFormat = cursor.blockFormat();

    textBlockFormat.setAlignment(Qt::AlignRight);
    cursor.mergeBlockFormat(textBlockFormat);

    getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Saved window geometry";

    settings->saveValue("ui", "geometry", this->geometry());

    for(int j = 0; j < tabWidgets->size(); j++)
    {
        for(int i = 0; i < tabWidgets->at(j)->count(); i++)
        {
            if(tabWidgets->at(j)->tabText(i).back() == '*')
            {
                QString message, temp;
                if(tabWidgets->at(j)->tabText(i).left(tabWidgets->at(j)->tabText(i).size() - 1) == "New Tab")
                {
                    message.setNum(j + 1);
                    temp.setNum(i + 1);
                    message = "New Tab " + message + " " + temp;
                }
                else
                {
                    message = tabWidgets->at(j)->tabText(i).left(tabWidgets->at(j)->tabText(i).size() - 1);
                }
                int clicked = QMessageBox::warning(this, "Save?", "Would you like to save " + message + "?", QMessageBox::Ok ,QMessageBox::No);
                if(clicked == QMessageBox::Ok)
                {
                    on_actionSave_triggered();
                    qDebug() << "Closing " + QString::number(i);
                }
                else if(clicked == QMessageBox::No)
                {
                    qDebug() << "Closing " + QString::number(i);
                }
            }
        }
    }

    QWidget::closeEvent(event);
}

void MainWindow::removeTabFromWidget(int widgetIndex, int tabIndex)
{
    tabWidgets->at(widgetIndex)->removeTab(tabIndex);

    if(widgetIndex > 0 && tabWidgets->at(widgetIndex)->count() == 1)
    {
        findChildren<QDockWidget*>().at(widgetIndex - 1)->hide();
    }
}

QVector<CustomTabWidget *> MainWindow::getTabWidgets()
{
    return *tabWidgets;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug() << "MainWindow drag enter event";
    QPoint contactPosition = mapFromGlobal(mapToGlobal(event->pos()));

    //If the contact point is showing some sort of desire to detach and is on the right side of the window
    if(contactPosition.x() > (size().width() * 0.75))
    {
        event->acceptProposedAction();
    }

    //If the tab is currently on the bottom side of the window
    if(contactPosition.y() > (size().height() * 0.75))
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << "Main window drop event detected";

    //Get the current docking widgets in the mainwindow
    QList<QDockWidget*> dockWidgets = findChildren<QDockWidget*>();

    //Get position of the event occuring
    QPoint dropPosition = mapFromGlobal(mapToGlobal(event->pos()));

    dock = new CustomDockWidget(this);


    //Get data from the clipboard with this mime data tag and then read the bytes to convert it to TabTransferData
    QByteArray data = event->mimeData()->data("application/tab");
    QDataStream ds(&data, QIODevice::ReadWrite);
    TabTransferData testTabData;

    ds >> testTabData;

    //This highlighter was allocated, however this shouldnt be the case as it has no rules so we must
    //explicitly assign it back to nullptr to avoid errors in other modules.
    if(testTabData.highlighter != nullptr && testTabData.highlighter->getRuleCount() == 0)
    {
        testTabData.highlighter = nullptr;
    }

    if(dropPosition.x() > (size().width() * 0.75))
    {
        bool isOccupingRight = false;
        QDockWidget* offender;
        for(QDockWidget* dock: dockWidgets)
        {
            isOccupingRight |= dockWidgetArea(dock) == Qt::RightDockWidgetArea;
            if(isOccupingRight)
            {
                offender = dock;
                break;
            }
        }

        //There was no dock widget occuping the right dock widget area
        if(!isOccupingRight)
        {
            //Make a new custom tab widget incase it is needed
            CustomTabWidget* tabWidget = new CustomTabWidget(this);

            QTextCursor cursor = tabWidget->getCurrentTabWidget()->getTextEdit()->textCursor();
            cursor.setPosition(testTabData.cursorPosition);

            QTextCharFormat format;
            format.setFont(testTabData.fontInformation);
            cursor.setCharFormat(format);

            //Set the widget
            tabWidget->getCurrentTabWidget()->getTextEdit()->setText(testTabData.text);
            tabWidget->setTabText(tabWidget->currentIndex(), testTabData.tabName);
            tabWidget->getCurrentTabWidget()->setSyntaxHighlighter(testTabData.highlighter);
            tabWidget->getCurrentTabWidget()->setTabsFileName(testTabData.filePath);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
            //tabWidget->getCurrentTabWidget()->setFont(testTabData.fontInformation);
            removeTabFromWidget(CustomTabWidget::tabParent, CustomTabWidget::tabRemoving);

            //connect signal
            QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setWindowToFileName);
            QObject::connect(tabWidget->getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);


            //set the dock widget incase it does get added
            dock->setWidget(tabWidget);

            addDockWidget(Qt::RightDockWidgetArea, dock);
            tabWidgets->push_back(tabWidget);
            tabWidgets->at(tabWidgets->size() - 1)->getCurrentTabWidget()->getTextEdit()->setFocus();
        }
        else //Add to right dock widget since one is already there
        {
            offender->show();

            CustomTabWidget* tabWidget = offender->findChild<CustomTabWidget*>();

            QTextCursor cursor = tabWidget->getCurrentTabWidget()->getTextEdit()->textCursor();
            cursor.setPosition(testTabData.cursorPosition);

            tabWidget->insertTab(tabWidget->count() - 1, new TextTabWidget(tabWidget), "New Tab");
            tabWidget->setCurrentIndex(tabWidget->count() - 2);
            tabWidget->getCurrentTabWidget()->setTabsFileName(testTabData.filePath);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setText(testTabData.text);
            tabWidget->setTabText(tabWidget->currentIndex(), testTabData.tabName);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setFocus();
            tabWidget->getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
            tabWidget->getCurrentTabWidget()->setFont(testTabData.fontInformation);
            removeTabFromWidget(CustomTabWidget::tabParent, CustomTabWidget::tabRemoving);

            QObject::connect(tabWidget->getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
        }

        setWindowTitle(testTabData.filePath);
    }
    else if(dropPosition.y() > (size().height() * 0.75))
    {
        bool isOccupingBottom = false;
        QDockWidget* offender;
        for(QDockWidget* dock: dockWidgets)
        {
            isOccupingBottom |= dockWidgetArea(dock) == Qt::BottomDockWidgetArea;
            if(isOccupingBottom)
            {
                offender = dock;
                break;
            }
        }

        //There was no dock widget occuping the right dock widget area
        if(!isOccupingBottom)
        {
            //Make a new custom tab widget incase it is needed
            CustomTabWidget* tabWidget = new CustomTabWidget(this);

            QTextCursor cursor = tabWidget->getCurrentTabWidget()->getTextEdit()->textCursor();
            cursor.setPosition(testTabData.cursorPosition);

            QTextCharFormat format;
            format.setFont(testTabData.fontInformation);

            cursor.setCharFormat(format);

            //Set the widget
            tabWidget->getCurrentTabWidget()->getTextEdit()->setText(testTabData.text);
            tabWidget->setTabText(tabWidget->currentIndex(), testTabData.tabName);
            tabWidget->getCurrentTabWidget()->setSyntaxHighlighter(testTabData.highlighter);
            tabWidget->getCurrentTabWidget()->setTabsFileName(testTabData.filePath);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
            tabWidget->getCurrentTabWidget()->setFont(testTabData.fontInformation);
            removeTabFromWidget(CustomTabWidget::tabParent, CustomTabWidget::tabRemoving);

            //connect signal
            QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::setWindowToFileName);
            QObject::connect(tabWidget->getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);


            //set the dock widget incase it does get added
            dock->setWidget(tabWidget);

            addDockWidget(Qt::BottomDockWidgetArea, dock);
            tabWidgets->push_back(tabWidget);
            tabWidgets->at(tabWidgets->size() - 1)->getCurrentTabWidget()->getTextEdit()->setFocus();
        }
        else //Add to right dock widget since one is already there
        {
            offender->show();
            CustomTabWidget* tabWidget = offender->findChild<CustomTabWidget*>();

            QTextCursor cursor = tabWidget->getCurrentTabWidget()->getTextEdit()->textCursor();
            cursor.setPosition(testTabData.cursorPosition);

            tabWidget->insertTab(tabWidget->count() - 1, new TextTabWidget(tabWidget), "New Tab");
            tabWidget->setCurrentIndex(tabWidget->count() - 2);
            tabWidget->getCurrentTabWidget()->setTabsFileName(testTabData.filePath);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setText(testTabData.text);
            tabWidget->setTabText(tabWidget->currentIndex(), testTabData.tabName);
            tabWidget->getCurrentTabWidget()->getTextEdit()->setFocus();
            tabWidget->getCurrentTabWidget()->getTextEdit()->setTextCursor(cursor);
            tabWidget->getCurrentTabWidget()->setFont(testTabData.fontInformation);
            removeTabFromWidget(CustomTabWidget::tabParent, CustomTabWidget::tabRemoving);

            QObject::connect(tabWidget->getCurrentTabWidget()->getTextEdit(), &QTextEdit::textChanged, this, &MainWindow::fileChanged);
        }

        setWindowTitle(testTabData.filePath);
    }

    CustomTabWidget::currentSelectedTabIndex = tabWidgets->size() - 1;
    qDebug() << "Current tab widget:" << CustomTabWidget::currentSelectedTabIndex;
}

void MainWindow::on_actionFind_triggered()
{
    getCurrentTabWidget()->revealReplaceBox();
}

Ui::MainWindow MainWindow::get_UI()
{
    return *ui;
}

