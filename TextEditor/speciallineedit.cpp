#include "speciallineedit.h"

SpecialLineEdit::SpecialLineEdit()
{

}

void SpecialLineEdit::focusInEvent(QFocusEvent *)
{
    QLineEdit::textChanged(this->text());
}
