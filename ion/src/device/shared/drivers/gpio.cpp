#include <ion/gpio.h>

#include <regs/regs.h>
#include <drivers/usb.h>

namespace Ion {
namespace GPIO {

class STM32Pin final : public Pin {
public:
  constexpr STM32Pin(Ion::Device::Regs::GPIO port, uint8_t pin) : m_port(port), m_pin(pin) {}

  void setPullNone() const override {
    m_port.PUPDR()->setPull(m_pin, Device::Regs::GPIO::PUPDR::Pull::None);
  }
  void setPullUp() const override {
    m_port.PUPDR()->setPull(m_pin, Device::Regs::GPIO::PUPDR::Pull::Up);
  }
  void setPullDown() const override {
    m_port.PUPDR()->setPull(m_pin, Device::Regs::GPIO::PUPDR::Pull::Down);
  }

  void setInput() const override {
    m_port.MODER()->setMode(m_pin, Device::Regs::GPIO::MODER::Mode::Input);
  }
  void setOutput() const override {
    m_port.MODER()->setMode(m_pin, Device::Regs::GPIO::MODER::Mode::Output);
  }

  bool value() const override {
    if (Ion::Device::Regs::OTG.GCCFG()->getPWRDWN()) {
      Device::USB::shutdownOTG();
    }
    return m_port.IDR()->get(m_pin);
  }
  void setValue(bool value) const override {
    if (Ion::Device::Regs::OTG.GCCFG()->getPWRDWN()) {
      Device::USB::shutdownOTG();
    }
    m_port.ODR()->set(m_pin, value);
  }

private:
  const Ion::Device::Regs::GPIO m_port;
  const uint8_t m_pin;
};

const static STM32Pin pins[] {
  { Device::Regs::GPIOA, 11 },
  { Device::Regs::GPIOA, 12 },
};

const Pin* getPin(size_t pin) {
  if (pin < (sizeof(pins) / sizeof(pins[0]))) {
    return &pins[pin];
  }
  return nullptr;
}

}
}
