#include "app.h"
#include <apps/i18n.h>

extern "C" {
#include <assert.h>
}

namespace Home {

const char * App::Descriptor::name() {
  return I18n::translate(I18n::Message::Apps);
}

const char * App::Descriptor::upperName() {
  return I18n::translate(I18n::Message::AppsCapital);
}

App * App::Snapshot::unpack(Container * container) {
  return new (container->currentAppBuffer()) App(this);
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

App::App(Snapshot * snapshot) :
  ::App(snapshot, &m_controller, I18n::Message::Warning),
  m_controller(&m_modalViewController, snapshot)
{
}

}
