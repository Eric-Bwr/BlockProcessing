#pragma once

#include <thread>
#include "../TimeRepresentationSystem/Clock.h"
#include "../Library.hpp"

namespace ts {
    class Thread {
    private:
        std::thread* thread = nullptr;
        trs::clock::Clock* clock = nullptr;

        bool active = false;
        uint64_t id = 0;
        void (*func)(Thread* thread) = nullptr;
        cs::BasicCallback<void, es::Event&> callback;
        bool _mute = false;
    public:
        Thread() = default;
        Thread(bool detach, void (*func)(Thread* thread));
        Thread(bool detach, void (*func)(Thread* thread), trs::clock::Clock* handleClock);
        Thread(bool detach, void (*eventFunc)(es::Event&), trs::clock::Clock* handleClock);
        Thread(bool detach, trs::clock::Clock* handleClock, cs::BasicCallback<void, es::Event&>* eventHandle);
        ~Thread();

        void start(bool detach, void (*func)(Thread* thread) = nullptr);
        void kill();
        void update();

        void setHandle(trs::clock::Clock* handleClock);

        void setEventHandling(void (*func)(es::Event&));
        void setEventHandling(cs::BasicCallback<void, es::Event&>* eventHandle);

        void mute();
        void unMute();

        bool isThreadActive();
        uint64_t getId();
        std::thread* getThread();
        trs::Time getThreadTime();

        static void sUpdate();
        static void killAll();
        static void muteAll();
        static void unMuteAll();
        static bool areThreadsActive();
    };
}
