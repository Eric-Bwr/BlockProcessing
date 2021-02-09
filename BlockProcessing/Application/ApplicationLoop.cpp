#include "Application.h"

void App::gameLoop() {
    running = true;
    updateClock.setTimedLoopDuration(60);
    renderClock.setTimedLoopDuration(60);

    while(running) {
        trs::clock::Clock::sUpdate();

        if(updateClock.getLoopedBoolPF()){
            double time = updateClock.getDeltaTime();
            _update(time);
        }
        if(renderClock.getLoopedBoolPF()){
            double time = renderClock.getDeltaTime();
            _render(time);
        }
        appWindow->updateWindow();
    }
}