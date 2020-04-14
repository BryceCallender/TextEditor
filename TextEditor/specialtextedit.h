#ifndef SPECIALTEXTEDIT_H
#define SPECIALTEXTEDIT_H

#include <QTextEdit>

//Forward declaration
class CustomTabWidget;


/*! \class SpecialTextEdit
    \brief QTextEdit with special functionality.

    This class is responsible for indicating which tab widget is selected when a text edit is clicked into.
 */
class SpecialTextEdit: public QTextEdit
{
public:
    SpecialTextEdit(QWidget* parent = nullptr);

    void focusInEvent(QFocusEvent *e) override;
private:
    CustomTabWidget* parentTabWidget;
};

#endif // SPECIALTEXTEDIT_H
