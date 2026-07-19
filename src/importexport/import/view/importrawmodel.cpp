/*
 * Audacity: A Digital Audio Editor
 */

#include "importrawmodel.h"

#include "framework/global/translation.h"

#include "au3-audio-devices/AudioIOBase.h"
#include "au3-file-formats/FileFormats.h"
#include "au3-project-rate/ProjectRate.h"

#include "au3wrap/au3types.h"

#include <algorithm>
#include <cstring>

using namespace au::importexport;

namespace {
// Keep the last used options for the next raw import (like AU3 did)
int s_lastEncoding = SF_FORMAT_PCM_16;
int s_lastEndianIndex = 3; // default endianness
int s_lastChannelsIndex = 0;
long long s_lastOffset = 0;
double s_lastPercent = 100.0;
double s_lastRate = 0.0; // -> project rate

int endianFromIndex(int index)
{
    switch (index) {
    case 1: return SF_ENDIAN_LITTLE;
    case 2: return SF_ENDIAN_BIG;
    case 3: return SF_ENDIAN_CPU;
    case 0:
    default: return SF_ENDIAN_FILE;
    }
}
}

ImportRawModel::ImportRawModel(QObject* parent)
    : QObject(parent), muse::Contextable(muse::iocCtxForQmlObject(this))
{
}

void ImportRawModel::init(const QString& path)
{
    m_path = path;

    // Build the list of raw-capable encodings, remembering the libsndfile
    // subtype for each entry
    m_encodingSubtypes.clear();
    m_encodingList.clear();

    const int num = sf_num_encodings();
    int selection = 0;

    for (int i = 0; i < num; ++i) {
        SF_INFO info;
        memset(&info, 0, sizeof(info));

        const int subtype = sf_encoding_index_to_subtype(i);
        info.format = SF_FORMAT_RAW + SF_ENDIAN_LITTLE + subtype;
        info.channels = 1;
        info.samplerate = 44100;

        if (sf_format_check(&info)) {
            m_encodingSubtypes.push_back(subtype);
            m_encodingList << QString::fromStdString(sf_encoding_index_name(i).ToStdString());

            if (s_lastEncoding == subtype) {
                selection = static_cast<int>(m_encodingSubtypes.size()) - 1;
            }
        }
    }

    m_encodingIndex = selection;
    m_endianIndex = s_lastEndianIndex;
    m_channelsIndex = s_lastChannelsIndex;
    m_offset = QString::number(s_lastOffset);
    m_percent = QString::number(s_lastPercent);

    // On first run, use the project sample rate by default
    double rate = s_lastRate;
    if (rate < 100.0) {
        au::au3::Au3Project* project = nullptr;
        if (globalContext()->currentProject()) {
            project = reinterpret_cast<au::au3::Au3Project*>(globalContext()->currentProject()->au3ProjectPtr());
        }
        rate = project ? ProjectRate::Get(*project).GetRate() : 44100.0;
    }

    m_sampleRateList.clear();
    int rateSelection = -1;
    for (int i = 0; i < AudioIOBase::NumStandardRates; ++i) {
        m_sampleRateList << AudioIOBase::StandardRates[i];
        if (AudioIOBase::StandardRates[i] == static_cast<int>(rate)) {
            rateSelection = i;
        }
    }

    if (rateSelection == -1) {
        // the project rate is not a standard rate, add it to the list
        m_sampleRateList << static_cast<int>(rate);
        rateSelection = m_sampleRateList.size() - 1;
    }

    m_sampleRateIndex = rateSelection;

    emit pathChanged();
    emit listsChanged();
    emit currentEncodingIndexChanged();
    emit currentEndianIndexChanged();
    emit currentChannelsIndexChanged();
    emit currentSampleRateIndexChanged();
    emit offsetChanged();
    emit percentChanged();

    updateValidity();
}

bool ImportRawModel::doImport()
{
    if (m_path.isEmpty() || m_encodingSubtypes.empty()) {
        return false;
    }

    RawImportParams params;
    params.encoding = currentEncoding();
    params.channels = static_cast<unsigned>(m_channelsIndex + 1);
    params.offset = std::max<long long>(m_offset.toLongLong(), 0);
    params.percent = std::clamp(m_percent.toDouble(), 0.0, 100.0);
    params.sampleRate = m_sampleRateList.at(m_sampleRateIndex).toDouble();

    const bool ok = importer()->importRawData(muse::io::path_t(m_path), params);
    if (ok) {
        s_lastEncoding = m_encodingSubtypes.at(m_encodingIndex);
        s_lastEndianIndex = m_endianIndex;
        s_lastChannelsIndex = m_channelsIndex;
        s_lastOffset = params.offset;
        s_lastPercent = params.percent;
        s_lastRate = params.sampleRate;
    }

    return ok;
}

QString ImportRawModel::fileName() const
{
    return muse::io::filename(muse::io::path_t(m_path)).toQString();
}

QVariantList ImportRawModel::encodingList() const
{
    return m_encodingList;
}

QVariantList ImportRawModel::endianList() const
{
    return {
        //: Refers to byte-order. Don't translate "endianness" if you don't know the correct technical word.
        muse::qtrc("importexport", "No endianness"),
        //: Refers to byte-order. Don't translate this if you don't know the correct technical word.
        muse::qtrc("importexport", "Little-endian"),
        //: Refers to byte-order. Don't translate this if you don't know the correct technical word.
        muse::qtrc("importexport", "Big-endian"),
        //: Refers to byte-order. Don't translate "endianness" if you don't know the correct technical word.
        muse::qtrc("importexport", "Default endianness"),
    };
}

QVariantList ImportRawModel::channelsList() const
{
    QVariantList out;
    out << muse::qtrc("importexport", "1 Channel (Mono)");
    out << muse::qtrc("importexport", "2 Channels (Stereo)");
    for (int i = 2; i < 16; ++i) {
        out << muse::qtrc("importexport", "%1 Channels").arg(i + 1);
    }
    return out;
}

QVariantList ImportRawModel::sampleRateList() const
{
    return m_sampleRateList;
}

int ImportRawModel::currentEncodingIndex() const
{
    return m_encodingIndex;
}

void ImportRawModel::setCurrentEncodingIndex(int index)
{
    if (m_encodingIndex == index || index < 0 || index >= static_cast<int>(m_encodingSubtypes.size())) {
        return;
    }

    m_encodingIndex = index;
    emit currentEncodingIndexChanged();

    updateValidity();
}

int ImportRawModel::currentEndianIndex() const
{
    return m_endianIndex;
}

void ImportRawModel::setCurrentEndianIndex(int index)
{
    if (m_endianIndex == index || index < 0 || index > 3) {
        return;
    }

    m_endianIndex = index;
    emit currentEndianIndexChanged();

    updateValidity();
}

int ImportRawModel::currentChannelsIndex() const
{
    return m_channelsIndex;
}

void ImportRawModel::setCurrentChannelsIndex(int index)
{
    if (m_channelsIndex == index || index < 0 || index >= 16) {
        return;
    }

    m_channelsIndex = index;
    emit currentChannelsIndexChanged();

    updateValidity();
}

int ImportRawModel::currentSampleRateIndex() const
{
    return m_sampleRateIndex;
}

void ImportRawModel::setCurrentSampleRateIndex(int index)
{
    if (m_sampleRateIndex == index || index < 0 || index >= m_sampleRateList.size()) {
        return;
    }

    m_sampleRateIndex = index;
    emit currentSampleRateIndexChanged();
}

QString ImportRawModel::offset() const
{
    return m_offset;
}

void ImportRawModel::setOffset(const QString& offset)
{
    if (m_offset == offset) {
        return;
    }

    m_offset = offset;
    emit offsetChanged();
}

QString ImportRawModel::percent() const
{
    return m_percent;
}

void ImportRawModel::setPercent(const QString& percent)
{
    if (m_percent == percent) {
        return;
    }

    m_percent = percent;
    emit percentChanged();
}

bool ImportRawModel::isValidFormat() const
{
    return m_isValidFormat;
}

int ImportRawModel::currentEncoding() const
{
    return m_encodingSubtypes.at(m_encodingIndex) | endianFromIndex(m_endianIndex);
}

void ImportRawModel::updateValidity()
{
    bool valid = false;

    if (!m_encodingSubtypes.empty()) {
        SF_INFO info;
        memset(&info, 0, sizeof(info));

        info.format = currentEncoding() | SF_FORMAT_RAW;
        info.channels = m_channelsIndex + 1;
        info.samplerate = 44100;

        valid = sf_format_check(&info) != 0;
    }

    if (m_isValidFormat != valid) {
        m_isValidFormat = valid;
        emit isValidFormatChanged();
    }
}
