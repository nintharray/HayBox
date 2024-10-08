#ifndef _INPUT_SWITCHMATRIXINPUT_HPP
#define _INPUT_SWITCHMATRIXINPUT_HPP

#include "core/InputSource.hpp"
#include "core/state.hpp"
#include "gpio.hpp"
#include "util/state_util.hpp"

#include <config.pb.h>

#define NA BTN_UNSPECIFIED

enum class DiodeDirection {
    ROW2COL,
    COL2ROW,
};

template <size_t num_rows, size_t num_cols> class SwitchMatrixInput : public InputSource {
  public:
    SwitchMatrixInput(
        const uint row_pins[num_rows],
        const uint col_pins[num_cols],
        const Button (&matrix)[num_rows][num_cols],
        DiodeDirection direction
    )
        : _matrix(matrix) {
        _direction = direction;

        if (_direction == DiodeDirection::ROW2COL) {
            _num_outputs = num_cols;
            _num_inputs = num_rows;
            _output_pins = col_pins;
            _input_pins = row_pins;
        } else {
            _num_outputs = num_rows;
            _num_inputs = num_cols;
            _output_pins = row_pins;
            _input_pins = col_pins;
        }

        // Initialize output pins.
        for (size_t i = 0; i < _num_outputs; i++) {
            gpio::init_pin(_output_pins[i], gpio::GpioMode::GPIO_INPUT_PULLUP);
        }

        // Initialize input pins.
        for (size_t i = 0; i < _num_inputs; i++) {
            gpio::init_pin(_input_pins[i], gpio::GpioMode::GPIO_INPUT_PULLUP);
        }
    }

    ~SwitchMatrixInput() {
        // Make sure all pins are set back to inputs.
        for (size_t i = 0; i < _num_outputs; i++) {
            gpio::init_pin(_output_pins[i], gpio::GpioMode::GPIO_INPUT_PULLUP);
        }
    }

    InputScanSpeed ScanSpeed() { return InputScanSpeed::FAST; }

    void UpdateInputs(InputState &inputs) {
        for (size_t i = 0; i < _num_outputs; i++) {
            // Activate the column/row.
            gpio::init_pin(_output_pins[i], gpio::GpioMode::GPIO_OUTPUT);
            gpio::write_digital(_output_pins[i], 0);

            // Read each cell in the column/row.
            for (size_t j = 0; j < _num_inputs; j++) {
                UpdateButtonState(
                    inputs,
                    _direction == DiodeDirection::ROW2COL ? j : i,
                    _direction == DiodeDirection::ROW2COL ? i : j,
                    !gpio::read_digital(_input_pins[j])
                );
            }

            // Deactivate the column/row.
            gpio::init_pin(_output_pins[i], gpio::GpioMode::GPIO_INPUT_PULLUP);
        }
    }

  protected:
    size_t _num_outputs;
    size_t _num_inputs;
    const uint *_output_pins;
    const uint *_input_pins;
    const Button (&_matrix)[num_rows][num_cols];
    DiodeDirection _direction;

  private:
    virtual void UpdateButtonState(
        InputState &inputs,
        size_t col_index,
        size_t row_index,
        bool pressed
    ) {
        Button button = _matrix[col_index][row_index];
        set_button(inputs.buttons, button, pressed);
    };
};

#endif