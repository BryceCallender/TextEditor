#include "speciallineedit.h"

SpecialLineEdit::SpecialLineEdit()
{

}

void SpecialLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);

    QLineEdit::textChanged(this->text());
}
