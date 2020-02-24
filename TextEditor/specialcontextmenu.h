#ifndef SPECIALCONTEXTMENU_H
#define SPECIALCONTEXTMENU_H

#include <QWidget>
#include <QMenu>


class SpecialContextMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SpecialContextMenu(QWidget *parent = nullptr);

    void clipboard_changed();
    void showContextPasteMenu(const QPoint& pos);
    void showContextMenu(const QPoint&);

    QString getSavedCopy(int index);
    void setSavedCopy(int index, const QString& text);

    int getTrack();
    void setTrack(int track);

private:
    static const qint8 numCopies = 3;
    QString savedCopy[numCopies] = {"", "", ""};
    int track;
    QMenu *pasteMenu;
};

#endif // SPECIALCONTEXTMENU_H
