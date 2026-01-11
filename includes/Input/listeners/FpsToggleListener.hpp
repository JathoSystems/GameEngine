#ifndef VUURJONGEN_WATERMEISJE_GAME_FPSTOGGLELISTENER_HPP
#define VUURJONGEN_WATERMEISJE_GAME_FPSTOGGLELISTENER_HPP
#include "Input/IKeyListener.h"

class FpsToggleListener : public IKeyListener {
private:
    bool _shown;

public:
    void onKeyPress(Key key) override;

    void onKeyRelease(Key key) override;
};

#endif //VUURJONGEN_WATERMEISJE_GAME_FPSTOGGLECOMPONENT_HPP
