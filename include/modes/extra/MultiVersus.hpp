#ifndef _MODES_MULTIVERSUS_HPP
#define _MODES_MULTIVERSUS_HPP

#include "core/ControllerMode.hpp"
#include "core/socd.hpp"
#include "core/state.hpp"

class MultiVersus : public ControllerMode {
  public:
    MultiVersus();

  protected:
    virtual void UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs);
    virtual void UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs);
};

#endif
