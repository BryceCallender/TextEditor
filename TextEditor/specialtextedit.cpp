#include "specialtextedit.h"
#include "customtabwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

SpecialTextEdit::SpecialTextEdit(QWidget *parent): QTextEdit(parent)
{
    parentTabWidget = reinterpret_cast<CustomTabWidget*>(parent->parentWidget());
}

void SpecialTextEdit::focusInEvent(QFocusEvent *e)
{
    qDebug() << "Focus event check";

    CustomTabWidget::currentSelectedTabIndex = parentTabWidget->tabWidgetIndex;

    MainWindow* mainWindow;

    //If this has no dock widget take this route
    if(dynamic_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget()->parentWidget()) != nullptr)
    {
        mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget()->parentWidget());
    }
    else //everything else is inside of a dock widget so it has one more parent to get through
    {
        mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget());
    }

    if(parentTabWidget->getCurrentTabWidget()->getSyntaxHighlighter() != nullptr)
    {
        mainWindow->get_UI().actionBold->setEnabled(false);
        mainWindow->get_UI().actionItalic->setEnabled(false);
        mainWindow->get_UI().actionUnderline->setEnabled(false);

        mainWindow->get_UI().actionAlign_Left->setEnabled(false);
        mainWindow->get_UI().actionCenter->setEnabled(false);
        mainWindow->get_UI().actionAlign_Right->setEnabled(false);

        mainWindow->get_UI().actionBullets->setEnabled(false);
        mainWindow->get_UI().actionNumbering->setEnabled(false);

        mainWindow->get_UI().fontComboBox->setEnabled(false);
        mainWindow->get_UI().fontSizeComboBox->setEnabled(false);
    }
    else
    {
        mainWindow->get_UI().actionBold->setEnabled(true);
        mainWindow->get_UI().actionItalic->setEnabled(true);
        mainWindow->get_UI().actionUnderline->setEnabled(true);


        mainWindow->get_UI().actionAlign_Left->setEnabled(true);
        mainWindow->get_UI().actionCenter->setEnabled(true);
        mainWindow->get_UI().actionAlign_Right->setEnabled(true);

        mainWindow->get_UI().actionBullets->setEnabled(true);
        mainWindow->get_UI().actionNumbering->setEnabled(true);

        mainWindow->get_UI().fontComboBox->setCurrentFont(SettingsManager::getInstance()->getValue("text/fontFamily").value<QFont>());
        mainWindow->get_UI().fontSizeComboBox->setCurrentText(SettingsManager::getInstance()->getValue("text/fontSize").toString());

        mainWindow->get_UI().fontComboBox->setEnabled(true);
        mainWindow->get_UI().fontSizeComboBox->setEnabled(true);
    }

    QTextEdit::focusInEvent(e);
}
