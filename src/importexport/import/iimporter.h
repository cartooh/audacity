#pragma once

#include "io/path.h"
#include "modularity/imoduleinterface.h"

#include "types/importtypes.h"
#include "trackedit/trackedittypes.h"

namespace au::importexport {
class IImporter : MODULE_EXPORT_INTERFACE
{
    INTERFACE_ID(IImporter)

public:
    virtual ~IImporter() = default;

    virtual FileInfo fileInfo(const muse::io::path_t& filePath) = 0;
    virtual bool import(const muse::io::path_t& filePath) = 0;
    virtual bool importIntoTrack(const muse::io::path_t& filePath, trackedit::TrackId dstTrackId, trackedit::secs_t startTime) = 0;
    virtual bool importFromSystemClipboard(const std::vector<muse::io::path_t>& filePaths, trackedit::secs_t startTime) = 0;
    virtual std::vector<std::string> supportedExtensions() const = 0;

    //! Whether the file extension suggests headerless (raw / PCM) audio data
    virtual bool isRawDataFile(const muse::io::path_t& filePath) const = 0;
    //! Opens the raw data import options dialog for the given file and imports it
    virtual bool importRaw(const muse::io::path_t& filePath) = 0;
    //! Imports the given file as headerless audio data with the given options
    virtual bool importRawData(const muse::io::path_t& filePath, const RawImportParams& params) = 0;
};
}
