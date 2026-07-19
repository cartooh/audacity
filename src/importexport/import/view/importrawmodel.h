/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include <QObject>

#include "framework/global/modularity/ioc.h"

#include "context/iglobalcontext.h"

#include "../iimporter.h"

namespace au::importexport {
class ImportRawModel : public QObject, public muse::Contextable
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName NOTIFY pathChanged)

    Q_PROPERTY(QVariantList encodingList READ encodingList NOTIFY listsChanged)
    Q_PROPERTY(QVariantList endianList READ endianList NOTIFY listsChanged)
    Q_PROPERTY(QVariantList channelsList READ channelsList NOTIFY listsChanged)
    Q_PROPERTY(QVariantList sampleRateList READ sampleRateList NOTIFY listsChanged)

    Q_PROPERTY(int currentEncodingIndex READ currentEncodingIndex WRITE setCurrentEncodingIndex NOTIFY currentEncodingIndexChanged)
    Q_PROPERTY(int currentEndianIndex READ currentEndianIndex WRITE setCurrentEndianIndex NOTIFY currentEndianIndexChanged)
    Q_PROPERTY(int currentChannelsIndex READ currentChannelsIndex WRITE setCurrentChannelsIndex NOTIFY currentChannelsIndexChanged)
    Q_PROPERTY(int currentSampleRateIndex READ currentSampleRateIndex WRITE setCurrentSampleRateIndex NOTIFY currentSampleRateIndexChanged)

    Q_PROPERTY(QString offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QString percent READ percent WRITE setPercent NOTIFY percentChanged)

    Q_PROPERTY(bool isValidFormat READ isValidFormat NOTIFY isValidFormatChanged)

    muse::ContextInject<au::context::IGlobalContext> globalContext{ this };
    muse::ContextInject<IImporter> importer{ this };

public:
    explicit ImportRawModel(QObject* parent = nullptr);

    Q_INVOKABLE void init(const QString& path);
    Q_INVOKABLE bool doImport();

    QString fileName() const;

    QVariantList encodingList() const;
    QVariantList endianList() const;
    QVariantList channelsList() const;
    QVariantList sampleRateList() const;

    int currentEncodingIndex() const;
    void setCurrentEncodingIndex(int index);

    int currentEndianIndex() const;
    void setCurrentEndianIndex(int index);

    int currentChannelsIndex() const;
    void setCurrentChannelsIndex(int index);

    int currentSampleRateIndex() const;
    void setCurrentSampleRateIndex(int index);

    QString offset() const;
    void setOffset(const QString& offset);

    QString percent() const;
    void setPercent(const QString& percent);

    bool isValidFormat() const;

signals:
    void pathChanged();
    void listsChanged();
    void currentEncodingIndexChanged();
    void currentEndianIndexChanged();
    void currentChannelsIndexChanged();
    void currentSampleRateIndexChanged();
    void offsetChanged();
    void percentChanged();
    void isValidFormatChanged();

private:
    int currentEncoding() const;
    void updateValidity();

    QString m_path;

    std::vector<int> m_encodingSubtypes;
    QVariantList m_encodingList;
    QVariantList m_sampleRateList;

    int m_encodingIndex = 0;
    int m_endianIndex = 0;
    int m_channelsIndex = 0;
    int m_sampleRateIndex = 0;
    QString m_offset = "0";
    QString m_percent = "100";

    bool m_isValidFormat = true;
};
}
