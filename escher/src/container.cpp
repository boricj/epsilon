#include <escher/container.h>
#include <ion.h>

Container::Container() :
  RunLoop(),
  m_activeApp(nullptr)
{
}

Container::~Container() {
  if (m_activeApp) {
    delete m_activeApp;
  }
}

void Container::switchTo(App::Snapshot * snapshot) {
  if (m_activeApp && snapshot == m_activeApp->snapshot()) {
    return;
  }
  if (m_activeApp) {
    m_activeApp->willBecomeInactive();
    m_activeApp->snapshot()->pack(m_activeApp);
  }
  if (snapshot) {
    m_activeApp = snapshot->unpack(this);
  } else {
    m_activeApp = nullptr;
  }
  if (m_activeApp) {
    m_activeApp->didBecomeActive(window());
  }
}

App * Container::activeApp() {
  return m_activeApp;
}

bool Container::dispatchEvent(Ion::Events::Event event) {
  if (m_activeApp->processEvent(event)) {
    windowRedraw();
    return true;
  }
  return false;
}

void Container::run() {
  window()->setFrame(KDRect(0, 0, Ion::Display::Width, Ion::Display::Height));
  window()->redraw();
  RunLoop::run();
}

void Container::windowRedraw() {
  window()->redraw();
}
