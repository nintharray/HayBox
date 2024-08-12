/* Ultimate profile by Taker */
#include "modes/Ultimate.hpp"

#define ANALOG_STICK_MIN 28
#define ANALOG_STICK_NEUTRAL 128
#define ANALOG_STICK_MAX 228

Ultimate::Ultimate() : ControllerMode() {}

void Ultimate::UpdateDigitalOutputs(const InputState &inputs, OutputState &outputs) {
    outputs.a = inputs.rt1;
    outputs.b = inputs.rf1;
    outputs.x = inputs.rf2;
    outputs.y = inputs.rf6;
    // outputs.buttonL = inputs.rf7;
    outputs.buttonR = inputs.rf3;
    outputs.triggerLDigital = inputs.lf4;
    outputs.triggerRDigital = inputs.rf5;
    outputs.start = inputs.mb1;
    outputs.select = inputs.mb3;
    outputs.home = inputs.mb2;

    // Turn on D-Pad layer by holding Mod X + Mod Y or Nunchuk C button.
    if ((inputs.lt1 && inputs.lt2) || inputs.nunchuk_c) {
        outputs.dpadUp = inputs.rt4;
        outputs.dpadDown = inputs.rt2;
        outputs.dpadLeft = inputs.rt3;
        outputs.dpadRight = inputs.rt5;

				// If MX + MY are held and ZL + ZR are pressed, send L + R signal.
				// This is just intended for resetting Training Mode in Ultimate.
				if (inputs.lf4 && inputs.lf5) {
					outputs.buttonL = inputs.lf4;
					outputs.buttonR = inputs.lf5;
				}
    }
}

void Ultimate::UpdateAnalogOutputs(const InputState &inputs, OutputState &outputs) {
    // Coordinate calculations to make modifier handling simpler.
    UpdateDirections(
        inputs.lf3, // Left
        inputs.lf1, // Right
        inputs.lf2, // Down
        inputs.rf4, // Up
        inputs.rt3, // C-Left
        inputs.rt5, // C-Right
        inputs.rt2, // C-Down
        inputs.rt4, // C-Up
        ANALOG_STICK_MIN,
        ANALOG_STICK_NEUTRAL,
        ANALOG_STICK_MAX,
        outputs
    );

    bool shielding = inputs.lf4 || inputs.rf5;

    if (inputs.lt1) {
        // MX + Horizontal = 6625 = 53
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 53);
        }
        // MX + Vertical = 44
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 43);
        }
        if (directions.diagonal) {
            // MX + q1/2/3/4 = 53 35
            outputs.leftStickX = 128 + (directions.x * 40);
            outputs.leftStickY = 128 + (directions.y * 26);
            if (shielding) {
                // MX + L, R, LS, and MS + q1/2/3/4 = 6375 3750 = 51 30
                outputs.leftStickX = 128 + (directions.x * 51);
                outputs.leftStickY = 128 + (directions.y * 30);
            }
        }

        // Up-angled C-stick with MX.
        if (directions.cx != 0) {
            outputs.rightStickX = 128 + (directions.cx * 56);
            outputs.rightStickY = 128 - 56;
        }
    }

    if (inputs.lt2) {
        // MY + Horizontal (even if shield is held) = 41
        if (directions.horizontal) {
            outputs.leftStickX = 128 + (directions.x * 26);
        }
        // MY + Vertical (even if shield is held) = 53
        if (directions.vertical) {
            outputs.leftStickY = 128 + (directions.y * 51);
        }
        if (directions.diagonal) {
            // MY + q1/2/3/4 = 35 59
            outputs.leftStickX = 128 + (directions.x * 38);
            outputs.leftStickY = 128 + (directions.y * 49);
            if (shielding) {
                // MY + L, R, LS, and MS + q1/2 = 38 70
                outputs.leftStickX = 128 + (directions.x * 68);
                outputs.leftStickY = 128 + (directions.y * 40);
            }
        }

        // Down-angled C-stick with MY.
        if (directions.cx != 0) {
            outputs.rightStickX = 128 + (directions.cx * 56);
            outputs.rightStickY = 128 + 56;
        }

    }

    if (inputs.lf4) {
        outputs.triggerLAnalog = 140;
    }

    if (inputs.rf5) {
        outputs.triggerRAnalog = 140;
    }

    // Shut off C-stick when using D-Pad layer.
    if ((inputs.lt1 && inputs.lt2) || inputs.nunchuk_c) {
        outputs.rightStickX = 128;
        outputs.rightStickY = 128;
    }

    // Nunchuk overrides left stick.
    if (inputs.nunchuk_connected) {
        outputs.leftStickX = inputs.nunchuk_x;
        outputs.leftStickY = inputs.nunchuk_y;
    }
}
