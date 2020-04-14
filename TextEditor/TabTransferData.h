#ifndef TABTRANSFERDATA_H
#define TABTRANSFERDATA_H

#include <QString>
#include "codesyntaxhighlighter.h"

/*! \class TabTransferData
    \brief All data needed for tab transfer

    This contains all the data that will be converted to byte array in order to be used with mimedata. The data will be transferred when dropped.
 */
struct TabTransferData {
    QString tabName;
    QString filePath;
    QString text;

    TabTransferData() = default;
    TabTransferData(const TabTransferData &) = default;
    ~TabTransferData() = default;

    //! DataStream extractor for mimedata retrieval of the class TabTransferData
    /*!
      \param in data stream that will be reading in the data.
      \param TabTransferData the tab data that will be populated with the stream data.
     */
    friend QDataStream& operator<<(QDataStream &out, const TabTransferData& tabData)
    {
        out << tabData.tabName << tabData.filePath << tabData.text;
        return out;
    }

    //! DataStream extractor for mimedata retrieval of the class TabTransferData
    /*!
      \param in data stream that will be reading in the data.
      \param TabTransferData the tab data that will be populated with the stream data.
     */
    friend QDataStream& operator>>(QDataStream &in, TabTransferData& tabData)
    {
        QString tabName;
        QString filePath;
        QString text;

        in >> tabName >> filePath >> text;

        tabData.tabName = tabName;
        tabData.filePath = filePath;
        tabData.text = text;

        return in;
    }
};

Q_DECLARE_METATYPE(TabTransferData);

#endif // TABTRANSFERDATA_H
