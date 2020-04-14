#ifndef SPECIALLINEEDIT_H
#define SPECIALLINEEDIT_H

#include <QLineEdit>

/*! \class SpecialLineEdit
    \brief QLineEdit with special functionality.

    This class is responsible for triggering a research on the document when focused
 */
class SpecialLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SpecialLineEdit(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *) override;
};

#endif // SPECIALLINEEDIT_H
