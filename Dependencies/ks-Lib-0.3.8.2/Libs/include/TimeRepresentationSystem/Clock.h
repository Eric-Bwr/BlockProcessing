// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once

#include "Time.h"

// namespace Time Representations System
namespace trs {
    // namespace clock
    namespace clock {

        enum ClockTask {
            UNKNOWN = -1,
            TIMEDBOOLEAN,
            LOOPEDBOOLEAN
        };

        class Clock {
        private:
            static std::vector<Clock*> clocks;
            static bool staticUpdate;
            bool firstClock = true;
            Time time;

            std::chrono::_V2::high_resolution_clock::time_point initTime;
            std::chrono::_V2::high_resolution_clock::time_point lastTime;
            std::chrono::_V2::high_resolution_clock::time_point startTime;
            std::chrono::_V2::high_resolution_clock::time_point endTime;
            std::chrono::duration<float> elapsedTime;

            bool timed = false;
            float tbDuration = 0;
            float tbTime = 0;

            bool looped = false;
            bool loopedBool = false;
            double loops = 0;
            float delta = 0, deltaTime = 0;
        public:
            Clock();
            explicit Clock(void*);
            Clock(ClockTask task, double value);
            ~Clock();

            void setTimedBoolDuration(float timeInSec);
            void setTimedLoopDuration(unsigned int loopsPerSec);
            bool getTimedBoolPF();
            bool getLoopedBoolPF();
            float getDeltaTime();
            bool isTimed();
            bool isLooped();


            Time getTime();
            void update();
            static void sUpdate();


        };
    } // namespace clock
} // namespace Time Representations System
