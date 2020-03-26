#ifndef SPECIALTEXTEDIT_H
#define SPECIALTEXTEDIT_H

#include <QTextEdit>

class CustomTabWidget;

class SpecialTextEdit: public QTextEdit
{
public:
    SpecialTextEdit(QWidget* parent = nullptr);

    void focusInEvent(QFocusEvent *e) override;
private:
    CustomTabWidget* parentTabWidget;
};

#endif // SPECIALTEXTEDIT_H
