#ifndef TABTRANSFERDATA_H
#define TABTRANSFERDATA_H

#include <QString>
#include "codesyntaxhighlighter.h"

struct TabTransferData {
    QString tabName;
    QString filePath;
    QString text;
    CodeSyntaxHighlighter* highlighter;

    friend QDataStream& operator<<(QDataStream &out, const TabTransferData& tabData)
    {
        out << tabData.tabName << tabData.filePath << tabData.text << tabData.highlighter;
        return out;
    }

    friend QDataStream& operator>>(QDataStream &in, TabTransferData& tabData)
    {
        QString tabName;
        QString filePath;
        QString text;
        CodeSyntaxHighlighter* highlighter;

        in >> tabName >> filePath >> text;

        in >> *highlighter;

        tabData.tabName = tabName;
        tabData.filePath = filePath;
        tabData.text = text;
        tabData.highlighter = highlighter;

        return in;
    }
};

Q_DECLARE_METATYPE(TabTransferData);

#endif // TABTRANSFERDATA_H
