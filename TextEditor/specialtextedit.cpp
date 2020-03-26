#include "specialtextedit.h"
#include "customtabwidget.h"

SpecialTextEdit::SpecialTextEdit(QWidget *parent): QTextEdit(parent)
{
    parentTabWidget = reinterpret_cast<CustomTabWidget*>(parent->parentWidget());
}

void SpecialTextEdit::focusInEvent(QFocusEvent *e)
{
    CustomTabWidget::currentSelectedTabIndex = parentTabWidget->tabWidgetIndex;

    QTextEdit::focusInEvent(e);
}
