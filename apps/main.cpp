#include "apps_container.h"
#include "global_preferences.h"
#include <poincare/init.h>

#define DUMMY_MAIN 0
#if DUMMY_MAIN

void ion_main(int argc, char * argv[]) {
  // Initialize the backlight
  Ion::Backlight::init();
  while (1) {
    Ion::Display::pushRectUniform(KDRect(0,0,10,10), KDColorRed);
    Ion::Timing::msleep(100);
    Ion::Display::pushRectUniform(KDRect(0,0,10,10), KDColorBlue);
    Ion::Timing::msleep(100);
  }
}

#else

void ion_main(int argc, char * argv[]) {
  // Initialize Poincare::TreePool::sharedPool
  Poincare::Init();

#if EPSILON_GETOPT
  for (int i=1; i<argc; i++) {
    if (argv[i][0] != '-' || argv[i][1] != '-') {
      continue;
    }
    /* Option should be given at run-time:
     * $ ./epsilon.elf --language fr
     */
    if (strcmp(argv[i], "--language") == 0 && argc > i+1) {
      const char * languageIdentifiers[] = {"none", "en", "fr", "es", "de", "pt"};
      const char * requestedLanguageId = argv[i+1];
      for (int i=0; i<sizeof(languageIdentifiers)/sizeof(languageIdentifiers[0]); i++) {
        if (strcmp(requestedLanguageId, languageIdentifiers[i]) == 0) {
          GlobalPreferences::sharedGlobalPreferences()->setLanguage((I18n::Language)i);
          break;
        }
      }
      continue;
    }
    /* Option should be given at run-time:
     * $ ./epsilon.elf --[app_name]-[option] [arguments]
     * For example:
     * $ make -j8 PLATFORM=emscripten EPSILON_APPS=code
     * $ ./epsilon.elf --code-script hello_world.py:print("hello") --code-lock-on-console
     */
    const char * appNames[] = {"home", EPSILON_APPS_NAMES};
    for (int j = 0; j < AppsContainer::sharedAppsContainer()->numberOfApps(); j++) {
      App::Snapshot * snapshot = AppsContainer::sharedAppsContainer()->appSnapshotAtIndex(j);
      int cmp = strcmp(argv[i]+2, appNames[j]);
      if (cmp == '-') {
        snapshot->setOpt(argv[i]+2+strlen(appNames[j])+1, argv[i+1]);
        break;
      }
    }
  }
#endif
  AppsContainer::sharedAppsContainer()->run();
}

#endif
