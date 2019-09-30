#include "external_app.h"
#include <apps/i18n.h>
#include <assert.h>

#include <escher/image.h>
#include "apps_container.h"

namespace ImageStore {
extern const Image * SettingsIcon;
}

const char * ExternalApp::Descriptor::name() {
  return upperName();
}

const char * ExternalApp::Descriptor::upperName() {
  return filename;
}

const Image * ExternalApp::Descriptor::icon() {
  if (fileicon) {
    return fileicon;
  }
  return ImageStore::SettingsIcon;
}

App * ExternalApp::Snapshot::unpack(Container * container) {
  return new (container->currentAppBuffer()) ExternalApp(this);
}

ExternalApp::Descriptor * ExternalApp::Snapshot::descriptor() {
  return &m_descriptor;
}

ExternalApp::ExternalApp(ExternalApp::Snapshot * snapshot) :
  ::App(snapshot, nullptr)
{
  KDIonContext::sharedContext()->setOrigin(KDPointZero);
  KDIonContext::sharedContext()->setClippingRect(KDRect(KDPointZero, Ion::Display::Width, Ion::Display::Height));
  AppsContainer * container = AppsContainer::sharedAppsContainer();
  Ion::Archive::executeFile(snapshot->descriptor()->name());
  container->switchTo(container->appSnapshotAtIndex(0));
}
