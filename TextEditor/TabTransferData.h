#ifndef TABTRANSFERDATA_H
#define TABTRANSFERDATA_H

#include <QString>
#include "codesyntaxhighlighter.h"

struct TabTransferData {
    QString tabName;
    QString filePath;
    QString text;
    CodeSyntaxHighlighter* highlighter;
    int cursorPosition;
    QFont fontInformation;

    friend QDataStream& operator<<(QDataStream &out, const TabTransferData& tabData)
    {
        out << tabData.tabName << tabData.filePath << tabData.text << tabData.highlighter << tabData.cursorPosition << tabData.fontInformation;
        return out;
    }

    friend QDataStream& operator>>(QDataStream &in, TabTransferData& tabData)
    {
        QString tabName;
        QString filePath;
        QString text;
        CodeSyntaxHighlighter* highlighter = nullptr;
        int cursorPosition;
        QFont fontInformation;

        in >> tabName >> filePath >> text;

        in >> *highlighter;

        in >> cursorPosition >> fontInformation;

        tabData.tabName = tabName;
        tabData.filePath = filePath;
        tabData.text = text;
        tabData.highlighter = highlighter;
        tabData.cursorPosition = cursorPosition;
        tabData.fontInformation = fontInformation;

        return in;
    }
};

Q_DECLARE_METATYPE(TabTransferData);

#endif // TABTRANSFERDATA_H
