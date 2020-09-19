#ifndef ION_GPIO_H
#define ION_GPIO_H

#include <stddef.h>

namespace Ion {
namespace GPIO {

class Pin {
protected:
  Pin() = default;

public:
  virtual void setPullNone() const = 0;
  virtual void setPullUp() const = 0;
  virtual void setPullDown() const = 0;

  virtual void setInput() const = 0;
  virtual void setOutput() const = 0;

  virtual bool value() const = 0;
  virtual void setValue(bool value) const = 0;
};

Pin const* getPin(size_t pin);

}
}

#endif
