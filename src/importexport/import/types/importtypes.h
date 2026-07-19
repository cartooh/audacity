/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include <QObject>

#include "io/path.h"

namespace au::importexport {
class TempoDetectionPref
{
    Q_GADGET
public:
    enum class TempoDetection {
        ALWAYS = 0,
        WORKSPACE_DEPENDENT,
        NEVER
    };
    Q_ENUM(TempoDetection)
};
struct FileInfo
{
    muse::io::path_t path;
    double duration = 0.0;
    int trackCount = 0;

    bool isEmpty() const noexcept
    {
        return path.empty();
    }
};

/// Persisted "remember my choice" action for tempo detection dialogs on import.
enum class LoopAction
{
    Ask,
    MatchProjectToLoop,
    MatchLoopToProject,
    DoNothing,
};

/// Options for importing headerless (raw / PCM) audio data,
/// see the Import Raw Data dialog.
struct RawImportParams
{
    /// libsndfile subtype combined with endianness flags
    /// (SF_FORMAT_SUBMASK | SF_FORMAT_ENDMASK part of an SF_INFO format)
    int encoding = 0;
    unsigned channels = 1;
    long long offset = 0; // bytes to skip before the audio data
    double percent = 100.0; // amount of the file to import
    double sampleRate = 44100.0;
};
}
