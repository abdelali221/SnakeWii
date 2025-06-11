#include "Input.h"
#include "WiiLibs.h"
#include "Variables.h"

int CheckWPAD() {
    WPAD_ScanPads();
	u32 pressed = WPAD_ButtonsDown(0);
    if (pressed & WPAD_BUTTON_UP) {
        if (WPADType) {
            return UP;
        } else {
            return LEFT;
        }
    } else if (pressed & WPAD_BUTTON_DOWN) {
        if (WPADType) {
            return DOWN;
        } else {
            return RIGHT;
        }
    } else if (pressed & WPAD_BUTTON_LEFT) {
        if (WPADType) {
            return LEFT;
        } else {
            return DOWN;
        }
    } else if (pressed & WPAD_BUTTON_RIGHT) {
        if (WPADType) {
            return RIGHT;
        } else {
            return UP;
        }
    } else if (pressed & WPAD_BUTTON_B) {
        return b_B;
    } else if (pressed & WPAD_BUTTON_A) {
        return b_A;
    } else if (pressed & WPAD_BUTTON_1) {
        return ONE;
    } else if (pressed & WPAD_BUTTON_2) {
        return TWO;
    } else if (pressed & WPAD_BUTTON_MINUS) {
        return MINUS;
    } else if (pressed & WPAD_BUTTON_PLUS) {
        return PLUS;
    } else if (pressed & WPAD_BUTTON_HOME) {
        return HOME;
    }
    return 0;
}