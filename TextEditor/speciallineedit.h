#ifndef SPECIALLINEEDIT_H
#define SPECIALLINEEDIT_H

#include <QLineEdit>

class SpecialLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SpecialLineEdit(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *) override;
};

#endif // SPECIALLINEEDIT_H
