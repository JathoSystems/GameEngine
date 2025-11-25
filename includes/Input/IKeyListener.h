#ifndef GAMEENGINE_IKEYLISTENER_H
#define GAMEENGINE_IKEYLISTENER_H

enum class Key {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    SPACE, ENTER, ESCAPE, TAB, BACKSPACE, DELETE_KEY,
    LEFT, RIGHT, UP, DOWN,
    SHIFT_LEFT, SHIFT_RIGHT, CTRL_LEFT, CTRL_RIGHT, ALT_LEFT, ALT_RIGHT,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
};

class IKeyListener {
public:
    virtual ~IKeyListener() = default;

    virtual void onKeyPress(Key key) = 0;

    virtual void onKeyRelease(Key key) = 0;
};

#endif
