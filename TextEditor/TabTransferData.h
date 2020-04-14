#ifndef TABTRANSFERDATA_H
#define TABTRANSFERDATA_H

#include <QString>
#include "codesyntaxhighlighter.h"

struct TabTransferData {
    QString tabName;
    QString filePath;
    QString text;
    int cursorPosition = 0;
    QFont fontInformation;

    TabTransferData() = default;
    TabTransferData(const TabTransferData &) = default;
    ~TabTransferData() = default;

    friend QDataStream& operator<<(QDataStream &out, const TabTransferData& tabData)
    {
        out << tabData.tabName << tabData.filePath << tabData.text << tabData.cursorPosition << tabData.fontInformation;
        return out;
    }

    friend QDataStream& operator>>(QDataStream &in, TabTransferData& tabData)
    {
        QString tabName;
        QString filePath;
        QString text;
        int cursorPosition;
        QFont fontInformation;

        in >> tabName >> filePath >> text;

        in >> cursorPosition >> fontInformation;

        tabData.tabName = tabName;
        tabData.filePath = filePath;
        tabData.text = text;
        tabData.cursorPosition = cursorPosition;
        tabData.fontInformation = fontInformation;

        return in;
    }
};

Q_DECLARE_METATYPE(TabTransferData);

#endif // TABTRANSFERDATA_H
