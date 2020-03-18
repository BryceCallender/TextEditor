#include "speciallineedit.h"

SpecialLineEdit::SpecialLineEdit(QWidget *parent): QLineEdit(parent)
{

}

void SpecialLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);

    QLineEdit::textChanged(this->text());
}
