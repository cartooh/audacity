/*
* Audacity: A Digital Audio Editor
*/

#include "importermodule.h"

#include <QtQml>

#include "modularity/ioc.h"
#include "framework/interactive/iinteractiveuriregister.h"

#include "internal/au3/au3importer.h"
#include "internal/importerconfiguration.h"

#include "view/importrawmodel.h"

#include "RegisterImportPlugins.h" // from au3/modules/import-export/ see IMPORT_EXPORT_MODULE in au3wrapDefs.cmake

using namespace au::importexport;
using namespace muse;

static const std::string mname("importer");

static void importer_init_qrc()
{
    Q_INIT_RESOURCE(importer);
}

ImporterModule::ImporterModule()
{
    RegisterImportPlugins();
}

std::string ImporterModule::moduleName() const
{
    return mname;
}

void ImporterModule::registerExports()
{
    m_configuration = std::make_shared<ImporterConfiguration>();

    globalIoc()->registerExport<IImporterConfiguration>(mname, m_configuration);
}

void ImporterModule::registerResources()
{
    importer_init_qrc();
}

void ImporterModule::registerUiTypes()
{
    qmlRegisterType<ImportRawModel>("Audacity.Import", 1, 0, "ImportRawModel");
}

void ImporterModule::resolveImports()
{
    auto ir = globalIoc()->resolve<muse::interactive::IInteractiveUriRegister>(mname);
    if (ir) {
        ir->registerQmlUri(Uri("audacity://project/importraw"), "Import/ImportRawDialog.qml");
    }
}

void ImporterModule::onInit(const muse::IApplication::RunMode&)
{
    m_configuration->init();
}

muse::modularity::IContextSetup* ImporterModule::newContext(const muse::modularity::ContextPtr& ctx) const
{
    return new ImporterContext(ctx);
}

// =====================================================
// ImporterContext
// =====================================================

void ImporterContext::registerExports()
{
    m_importer = std::make_shared<Au3Importer>(iocContext());

    ioc()->registerExport<IImporter>(mname, m_importer);
}

void ImporterContext::onInit(const muse::IApplication::RunMode&)
{
}

void ImporterContext::onDeinit()
{
}
