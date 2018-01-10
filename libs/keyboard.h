#include <stdint.h>

class Keyboard_Class {
    public:
        void update();
        struct _KEYCODES {
            enum _KEYCODES_ {
                ESC, K1, K2, K3, K4, K5, K6, K7,
                K8, K9, K0, MINUS, EQUAL, BACKSPACE, TAB, Q,
                W, E, R, T, Y, U, I, O,
                P, LBRACKET, RBRACKET, ENTER, LCTRL, A, S, D,
                F, G, H, J, K, L, SEMICOLMUMN, QUOTE,
                BACKTICK, LSHIFT, BACKSLASH, Z, X, C, V, B,
                N, M, COMMA, DOT, FSLASH, RSHIFT, KPMULT, LALT,
                SPACE, CAPSLOCK, F1, F2, F3, F4, F5, F6,
                F7, F8, F9, F10, NUMLOCK, SCRLOCK, KP7, KP8,
                KP9, KPMINUS, KP4, KP5, KP6, KPPLUS, KP1,
                KP2, KP3, KP0, KPDOT, F11, F12
            };
        } KEYCODES;
};

extern Keyboard_Class Keyboard;