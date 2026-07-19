/*
* Audacity: A Digital Audio Editor
*/

#pragma once

#include "modularity/ioc.h"
#include "context/iglobalcontext.h"
#include "importexport/import/iimporter.h"
#include "importexport/labels/ilabelsimporter.h"
#include "trackedit/itracksinteraction.h"

namespace au::projectscene {
class DropController : public QObject, public muse::Contextable
{
    Q_OBJECT

    muse::ContextInject<au::context::IGlobalContext> globalContext{ this };
    muse::ContextInject<importexport::IImporter> importer{ this };
    muse::ContextInject<importexport::ILabelsImporter> labelsImporter{ this };
    muse::ContextInject<trackedit::ITracksInteraction> tracksInteraction{ this };

public:
    explicit DropController(QObject* parent = nullptr);

    Q_INVOKABLE void probeAudioFiles(const QStringList& fileUrls);
    Q_INVOKABLE QVariantList lastProbedDurations() const;
    Q_INVOKABLE QVariantList lastProbedFileNames() const;
    Q_INVOKABLE void startImportDrag();
    Q_INVOKABLE void endImportDrag();
    Q_INVOKABLE int requiredTracksCount() const;
    Q_INVOKABLE void prepareConditionalTracks(int currentTrackId, int draggedFileCount);
    Q_INVOKABLE QVariantList draggedTracksIds(int currentTrackId, int draggedFilesCount);
    Q_INVOKABLE void removeDragAddedTracks(int currentTrackId, int draggedFilesCount);
    Q_INVOKABLE void handleDroppedFiles(const std::vector<trackedit::TrackId>& trackIds, double startTime);

private:
    std::vector<au::importexport::FileInfo> m_lastDraggedFilesInfo;
    QStringList m_lastDraggedUrls;
    //! Dragged files that are not imported into audio tracks directly,
    //! but via their own importers (raw data and label files)
    std::vector<muse::io::path_t> m_lastDraggedNonAudioPaths;
    int m_tracksCountWhenDragStarted = -1;
};
}
