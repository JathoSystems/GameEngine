#ifndef VUURJONGEN_WATERMEISJE_GAME_SPEEDTOGGLELISTENER_HPP
#define VUURJONGEN_WATERMEISJE_GAME_SPEEDTOGGLELISTENER_HPP
#include "Input/IKeyListener.h"

class SpeedToggleListener : public IKeyListener {
public:
    void onKeyPress(Key key) override;

    void onKeyRelease(Key key) override;
};

#endif //VUURJONGEN_WATERMEISJE_GAME_SPEEDTOGGLE_HPP
