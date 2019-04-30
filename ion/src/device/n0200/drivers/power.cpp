#include <drivers/power.h>
#include <drivers/board.h>
#include <drivers/reset.h>

namespace Ion {
namespace Power {

void __attribute__((noinline)) standby() {
  Device::Power::waitUntilOnOffKeyReleased();
  Device::Power::standbyConfiguration();
  Device::Board::shutdown();
  Device::Power::enterLowPowerMode();
  Device::Reset::core();
}

}
}